#include "stdafx.h"
#include "RBManifest.h"
#include "RigidBody.h"
#include "LineSegment.h"
#include "Line.h"
#include "PolygonShape.h"
#include "Vector2.h"

namespace RGPhysics
{
	RBManifest::RBManifest(Vector2 collisionAxis, RigidBody * rbA, RigidBody * rbB)
	{
		this->rbA = rbA;
		this->rbB = rbB;
		this->collisionAxis = collisionAxis;
	}

	void RBManifest::SettleCollisionPass()
	{
		std::vector<float> impulse_magnitudes = applyImpulse();
		applyFriction(impulse_magnitudes);
		applyPositionalCorrection();
	}

	std::vector<float> RBManifest::applyImpulse()
	{
		//Apply individually
		std::vector<float> impulse_magnitudes;
		for (int i = 0; i < this->contact_points.size(); i++)
		{
			Vector2 contact_point = std::get<0>(contact_points[i]);
			float impulse_magnitude = this->GetImpulseMagnitude(contact_points[i], this->collisionAxis);
			impulse_magnitudes.push_back(impulse_magnitude);

			//Apply impulse to bodies
			this->rbA->ApplyImpulse(this->collisionAxis * -impulse_magnitude, contact_point);
			this->rbB->ApplyImpulse(this->collisionAxis * impulse_magnitude, contact_point);
		}

		return impulse_magnitudes;
	}

	const float POSITIONAL_CORRECTION = 0.2f;
	const float POSITIONAL_SLOP = 0.01f;
	void RBManifest::applyPositionalCorrection()
	{
		if (contact_points.size() > 0)
		{
			//Get maximum depth
			float maximum_depth = std::get<1>(this->contact_points[0]);
			for (int i = this->contact_points.size() - 1; i > 0; i--)
			{
				float depth = std::get<1>(this->contact_points[i]);
				if (depth > maximum_depth)
					maximum_depth = depth;
			}

			float correction_magnitude = (max(maximum_depth - POSITIONAL_SLOP, 0) * POSITIONAL_CORRECTION) * (this->rbA->inv_mass + this->rbB->inv_mass);
			this->rbA->GetCollisionPolygon()->Translate(this->collisionAxis * correction_magnitude * this->rbA->inv_mass);
			this->rbB->GetCollisionPolygon()->Translate(this->collisionAxis * -correction_magnitude * this->rbB->inv_mass);
		}
	}

	void RBManifest::applyFriction(std::vector<float> impulse_magnitudes)
	{
		//Calculate friction coefficients
		float static_friction = sqrt(pow(this->rbA->COF_static, 2) + pow(this->rbB->COF_static, 2));
		float dynamic_friction = sqrt(pow(this->rbA->COF_dynamic, 2) + pow(this->rbB->COF_dynamic, 2));

		//Apply individually
		for (int i = 0; i < this->contact_points.size(); i++)
		{
			Vector2 localVelocityA = this->rbA->GetVelocity() + this->rbA->GetTangentialVelocity(std::get<0>(contact_points[i]));
			Vector2 localVelocityB = this->rbB->GetVelocity() + this->rbB->GetTangentialVelocity(std::get<0>(contact_points[i]));
			Vector2 relativeVelocity = localVelocityB - localVelocityA;

			Vector2 collision_tangent = relativeVelocity - (this->collisionAxis * Vector2::Dot(relativeVelocity, this->collisionAxis));
			collision_tangent.Normalize();

			//Calculate the impulse required to completely stop the sliding motion
			float necessary_impulse = this->GetImpulseMagnitude(contact_points[i], collision_tangent);
			if (abs(necessary_impulse) > abs(impulse_magnitudes[i] * static_friction))
			{
				necessary_impulse = impulse_magnitudes[i] * dynamic_friction;
			}

			this->rbA->ApplyImpulse(collision_tangent * -necessary_impulse, std::get<0>(contact_points[i]));
			this->rbB->ApplyImpulse(collision_tangent * necessary_impulse, std::get<0>(contact_points[i]));
		}
	}

	float RBManifest::GetImpulseMagnitude(std::tuple<Vector2, float> contact_point_tuple, Vector2 impulse_axis)
	{
		Vector2 contact_point = std::get<0>(contact_point_tuple);
		float penetration_depth = std::get<1>(contact_point_tuple);

		//Get radius unit vectors
		Vector2 radiusA = contact_point - this->rbA->GetCollisionPolygon()->GetCenterPoint();
		Vector2 radiusB = contact_point - this->rbB->GetCollisionPolygon()->GetCenterPoint();
		radiusA.Normalize();
		radiusB.Normalize();

		//Get COR and rel velocity
		float COR = min(this->rbA->COR, this->rbB->COR);
		Vector2 localVelocityA = this->rbA->GetVelocity() + this->rbA->GetTangentialVelocity(contact_point);
		Vector2 localVelocityB = this->rbB->GetVelocity() + this->rbB->GetTangentialVelocity(contact_point);
		Vector2 relativeVelocity = localVelocityB - localVelocityA;

		if (Vector2::Dot(relativeVelocity, impulse_axis) > 0)
		{
			//Apply to impulse formula
			float numerator = -(1 + COR) * Vector2::Dot(relativeVelocity, impulse_axis);
			float denominator = this->rbA->inv_mass + this->rbB->inv_mass;
			denominator += pow(Vector2::Cross(radiusA, impulse_axis), 2) * this->rbA->inv_rotational_inertia;
			denominator += pow(Vector2::Cross(radiusB, impulse_axis), 2) * this->rbB->inv_rotational_inertia;

			return numerator / denominator;
		}
		else return 0;
	}

	std::vector<std::tuple<Vector2, float>> RBManifest::GetContactPoints()
	{
		//Get best edges
		LineSegment bestEdgeA = GetBestEdge(*rbA, collisionAxis);
		LineSegment bestEdgeB = GetBestEdge(*rbB, collisionAxis * -1);

		//Find reference and incident edges (reference is clipper, most perpendicular)
		std::vector<Vector2> endPoints = bestEdgeA.GetEndPoints();
		Vector2 segmentAVector = endPoints[1] - endPoints[0];
		segmentAVector.Normalize();
		float segmentACross = Vector2::Cross(segmentAVector, collisionAxis);
		endPoints = bestEdgeB.GetEndPoints();
		Vector2 segmentBVector = endPoints[1] - endPoints[0];
		segmentBVector.Normalize();
		float segmentBCross = Vector2::Cross(segmentBVector, collisionAxis);
		LineSegment referenceEdge = bestEdgeA;
		RigidBody *referenceRB = rbA;
		LineSegment incidentEdge = bestEdgeB;
		RigidBody *incidentRB = rbB;
		if (abs(segmentBCross) > abs(segmentACross))
		{
			referenceEdge = bestEdgeB;
			referenceRB = rbB;
			incidentEdge = bestEdgeA;
			incidentRB = rbA;
		}

		//Clip incident edge
		std::vector<Vector2> referenceEndPoints = referenceEdge.GetEndPoints();
		Vector2 refOrthogonal = referenceEdge.GetLine().ToVector().GetOrthogonal(1);
		this->ClipSegment(Line(referenceEndPoints[0], referenceEndPoints[0] + refOrthogonal), referenceEndPoints[1], incidentEdge);
		this->ClipSegment(Line(referenceEndPoints[1], referenceEndPoints[1] + refOrthogonal), referenceEndPoints[0], incidentEdge);

		//Get contact points from clipped incident edge
		std::vector<Vector2> incidentEndPoints = incidentEdge.GetEndPoints();
		std::vector<std::tuple<Vector2, float>> contactPoints;
		Line referenceLine = referenceEdge.GetLine();
		Vector2 referenceCenterPoint = referenceRB->GetCollisionPolygon()->GetCenterPoint();
		float referenceDot = Vector2::Dot(refOrthogonal, referenceEndPoints[0]);
		for (int i = incidentEndPoints.size() - 1; i >= 0; i--)
		{
			if (Line::IsSameSide(referenceLine, referenceCenterPoint, incidentEndPoints[i]))
			{
				//Get collision depth
				float incidentDot = Vector2::Dot(refOrthogonal, incidentEndPoints[i]);
				contactPoints.push_back(std::tuple<Vector2, float>(incidentEndPoints[i], abs(incidentDot - referenceDot)));
			}
		}

		this->contact_points = contactPoints;
		return contactPoints;
	}

	void RBManifest::ClipSegment(Line clipLine, Vector2 sameSidePoint, LineSegment & segment)
	{
		//Determine if clip line intersects segment
		std::tuple<bool, Vector2> intersectionResult = Line::FindIntersection(clipLine, segment.GetLine());
		if (std::get<0>(intersectionResult) && segment.ContainsLinePoint(std::get<1>(intersectionResult)))
		{
			//Intersects, find new segment
			std::vector<Vector2> endPoints = segment.GetEndPoints();
			if (Line::IsSameSide(clipLine, sameSidePoint, endPoints[0]))
			{
				segment.SetEndPoints(endPoints[0], std::get<1>(intersectionResult));
			}
			else
			{
				segment.SetEndPoints(std::get<1>(intersectionResult), endPoints[1]);
			}
		}
	}

	LineSegment RBManifest::GetBestEdge(RigidBody & rb, Vector2 collisionAxis)
	{
		std::vector<Vector2> vertices = rb.GetCollisionPolygon()->GetVertices();
		int verticesSize = vertices.size();

		//Find minimum vertex dot
		int minVertex = -1;
		float minDot;
		for (int i = 0; i < verticesSize; i++)
		{
			float vertexDot = Vector2::Dot(vertices[i], collisionAxis);
			if (minVertex == -1 || vertexDot < minDot)
			{
				minVertex = i;
				minDot = vertexDot;
			}
		}

		//Get adjacent segments
		LineSegment segmentA(vertices[((minVertex - 1) + verticesSize) % verticesSize], vertices[minVertex]);
		LineSegment segmentB(vertices[(minVertex + 1) % verticesSize], vertices[minVertex]);

		//Find most perpendicular (max cross product)
		std::vector<Vector2> endPoints = segmentA.GetEndPoints();
		Vector2 segmentAVector = endPoints[1] - endPoints[0];
		segmentAVector.Normalize();
		float segmentACross = Vector2::Cross(segmentAVector, collisionAxis);
		endPoints = segmentB.GetEndPoints();
		Vector2 segmentBVector = endPoints[1] - endPoints[0];
		segmentBVector.Normalize();
		float segmentBCross = Vector2::Cross(segmentBVector, collisionAxis);

		return (abs(segmentACross) > abs(segmentBCross)) ? segmentA : segmentB;
	}
}