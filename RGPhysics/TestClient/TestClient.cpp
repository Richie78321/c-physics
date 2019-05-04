// TestClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "RGPhysics.h"
#include <SFML/Graphics.hpp>
#include <vector>

sf::ConvexShape getPolygonShape(Polygon *polygon)
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

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 450), "Physics Client");
	window.setFramerateLimit(90);

	Polygon *testPolygon = new Polygon(Polygon::GetRectVertices(10, 10, 200, 100));
	
	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Rotate polygon (test)
		testPolygon->Rotate(testPolygon->GetCenterPoint(), .01f);

		window.clear();
		
		window.draw(getPolygonShape(testPolygon));
		
		window.display();
	}

	return 0;
}