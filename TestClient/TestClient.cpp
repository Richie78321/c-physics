// TestClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "RGPhysics.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <tuple>

using namespace RGPhysics;

sf::ConvexShape getPolygonShape(PolygonShape *polygon)
{
	sf::ConvexShape shape;
	std::vector<Vector2> vertices = polygon->GetVertices();
	int numVertices = vertices.size();

	shape.setPointCount(numVertices);
	for (int i = 0; i < numVertices; i++)
	{
		shape.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y));
	}

	return shape;
}

std::vector<sf::Vertex> getLineVertices(Line line, sf::RenderWindow *window)
{
	std::vector<sf::Vertex> vertices;

	std::tuple<bool, RGPhysics::Vector2> result = line.PointAtX(0);
	if (!std::get<0>(result))
	{
		result = line.PointAtY(0);
	}
	vertices.push_back(sf::Vertex(sf::Vector2f(std::get<1>(result).x, std::get<1>(result).y)));

	result = line.PointAtX(window->getSize().x);
	if (!std::get<0>(result))
	{
		result = line.PointAtY(window->getSize().y);
	}
	vertices.push_back(sf::Vertex(sf::Vector2f(std::get<1>(result).x, std::get<1>(result).y)));

	return vertices;
}

void drawLine(Line line, sf::RenderWindow *window)
{
	std::vector<sf::Vertex> vertices = getLineVertices(line, window);
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i].color = sf::Color(255, 0, 0);
	}

	sf::Vertex vertexList[] = { vertices[0], vertices[1] };
	window->draw(vertexList, 2, sf::Lines);
}

void drawLineSegment(LineSegment lineSegment, sf::RenderWindow *window, sf::Color drawColor)
{
	std::vector<Vector2> endPoints = lineSegment.GetEndPoints();
	sf::Vertex vertexList[] = { sf::Vertex(sf::Vector2f(endPoints[0].x, endPoints[0].y), drawColor), sf::Vertex(sf::Vector2f(endPoints[1].x, endPoints[1].y), drawColor) };
	window->draw(vertexList, 2, sf::Lines);
}

void Update(SimManager &simManager, float elapsedMillis);
void Draw(SimManager &simManager, sf::RenderWindow &window);

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 450), "Physics Client");
	window.setFramerateLimit(90);

	SimManager simManager;
	RBCollider collider;
	simManager.AddSimElement(&collider);
	
	PolygonShape shape1(0, 300, 750, 50);
	PolygonShape shape2(Vector2(400, 0), 8, 50);

	shape1.Rotate(shape1.GetCenterPoint(), .1f);

	RigidBody rbA(&shape1, 0, .2f, .35f, .25f);
	RigidBody rbB(&shape2, 3, .2f, .35f, .25f);
	
	simManager.AddSimElement(&rbA);
	simManager.AddSimElement(&rbB);

	//shape1.Rotate(shape1.GetCenterPoint(), 3.14f / 6);
	//shape2.Rotate(shape2.GetCenterPoint(), 3.14f / 6);

	//rbA.AddAngularVelocity(-.003f);
	rbB.AddAngularVelocity(.005f);

	//rbB.AddVelocity(Vector2(.1f, 0));
	//rbB.ApplyImpulse(Vector2(3.0f, 0), Vector2(0, 200));

	sf::Clock clock;
	float acc = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			rbB.SetVelocity(Vector2(mousePos.x, mousePos.y) - shape2.GetCenterPoint());
		}
		
		rbB.AddVelocity(Vector2(0, 0.04f));

		simManager.Update();

		window.clear();
		
		window.draw(getPolygonShape(&shape1));
		sf::ConvexShape shape2Shape = getPolygonShape(&shape2);
		
		std::tuple<bool, Vector2, float, Vector2> SATResult = PolygonShape::AreIntersecting(shape1, shape2);
		if (!PolygonShape::ContainsPoint(shape2, Vector2(mousePos.x, mousePos.y)))
		{
			shape2Shape.setFillColor(sf::Color::White);
		}
		else
		{
			shape2Shape.setFillColor(sf::Color::Green);
		}
		window.draw(shape2Shape);

		//Vector2 tangentialVelocity = rbA.GetTangentialVelocity(rbA.GetCollisionPolygon()->GetVertices()[0]);
		//drawLineSegment(LineSegment(rbA.GetCollisionPolygon()->GetVertices()[0], rbA.GetCollisionPolygon()->GetVertices()[0] + (tangentialVelocity * 1000)), &window, sf::Color::Red);

		std::vector<LineSegment> sideSegments = rbA.GetCollisionPolygon()->GetSideSegments();
		std::vector<LineSegment> sideSegments2 = rbB.GetCollisionPolygon()->GetSideSegments();
		sideSegments.insert(sideSegments.end(), sideSegments2.begin(), sideSegments2.end());
		for (int i = 0; i < sideSegments.size(); i++)
		{
			drawLineSegment(sideSegments[i], &window, sf::Color::Black);
		}

		if (std::get<0>(SATResult))
		{
			//LineSegment collisionAxis(std::get<3>(SATResult), std::get<3>(SATResult) + (std::get<1>(SATResult) * std::get<2>(SATResult)));
			//drawLineSegment(collisionAxis, &window);
		}

		if (collider.tempStorage.size() > 0)
		{
			//Draw best segments
			//LineSegment bestSegment1 = collider.tempStorage[0].GetBestEdge(*collider.tempStorage[0].rbA, collider.tempStorage[0].collisionAxis);
			//LineSegment bestSegment2 = collider.tempStorage[0].GetBestEdge(*collider.tempStorage[0].rbB, collider.tempStorage[0].collisionAxis * -1);
			//drawLineSegment(bestSegment1, &window);
			//drawLineSegment(bestSegment2, &window);
			std::vector<std::tuple<Vector2, float>> contactPoints = collider.tempStorage[0].GetContactPoints();
			Vector2 collisionAxis = collider.tempStorage[0].collisionAxis;
			for (int i = contactPoints.size() - 1; i >= 0; i--)
			{
				sf::CircleShape pointCircle(2.5f);
				pointCircle.setPosition(std::get<0>(contactPoints[i]).x - 2.5f, std::get<0>(contactPoints[i]).y - 2.5f);
				pointCircle.setFillColor(sf::Color::Red);
				window.draw(pointCircle);
				drawLineSegment(LineSegment(std::get<0>(contactPoints[i]), std::get<0>(contactPoints[i]) + (collisionAxis * std::get<1>(contactPoints[i]))), &window, sf::Color::Red);
				drawLineSegment(LineSegment(std::get<0>(contactPoints[i]), std::get<0>(contactPoints[i]) - (collisionAxis * std::get<1>(contactPoints[i]))), &window, sf::Color::Green);
			}
		}

		window.display();
	}

	return 0;
}

void Update(RGPhysics::SimManager &simManager, float elapsedMillis)
{
	simManager.Update(elapsedMillis);
}

void Draw(SimManager &simManager, sf::RenderWindow &window)
{
	for (int i = simManager.GetSimElements().size() - 1; i >= 0; i--)
	{
		if (RigidBody *r = dynamic_cast<RigidBody*>(simManager.GetSimElements()[i]))
		{
			window.draw(getPolygonShape(r->GetCollisionPolygon()));
		}
	}
}