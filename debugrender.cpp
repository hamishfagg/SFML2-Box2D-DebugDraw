/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "debugrender.h"

DebugDraw::DebugDraw(sf::RenderWindow &window)
{
    this->window = &window;
}

DebugDraw::~DebugDraw()
{

}

//convert a Box2D (float 0.0f - 1.0f range) color to a SFML color (uint8 0 - 255 range)
sf::Color DebugDraw::B2SFColor(const b2Color &color, int alpha = 255)
{
	sf::Color result((sf::Uint8)(color.r*255), (sf::Uint8)(color.g*255), (sf::Uint8)(color.b*255), (sf::Uint8) alpha);
	return result;
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color)
{
	sf::ConvexShape polygon(4);

    polygon.setPoint(0, sf::Vector2f(aabb->lowerBound.x*RATIO, aabb->lowerBound.y*RATIO));
    polygon.setPoint(1, sf::Vector2f(aabb->upperBound.x*RATIO, aabb->lowerBound.y*RATIO));
    polygon.setPoint(2, sf::Vector2f(aabb->upperBound.x*RATIO, aabb->upperBound.y*RATIO));
    polygon.setPoint(3, sf::Vector2f(aabb->lowerBound.x*RATIO, aabb->upperBound.y*RATIO));

    polygon.setFillColor(this->B2SFColor(color, 50));
    polygon.setOutlineColor(this->B2SFColor(color));
    polygon.setOutlineThickness(1.0f);
	this->window->draw(polygon);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
    float lineProportion = 0.4f;
    b2Vec2 p1 = xf.p, p2;

	//red (X axis)
	p2 = p1 + (lineProportion * xf.q.GetXAxis());
    this->DrawSegment(p1, p2, b2Color(255, 0, 0));

	//green (Y axis)
	p2 = p1 + (lineProportion * xf.q.GetYAxis());
    this->DrawSegment(p1, p2, b2Color(0, 255, 0));
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    sf::VertexArray line;
    line.append(sf::Vertex(sf::Vector2f(p1.x*RATIO, p1.y*RATIO), this->B2SFColor(color)));
    line.append(sf::Vertex(sf::Vector2f(p2.x*RATIO, p2.y*RATIO), this->B2SFColor(color)));
    line.setPrimitiveType(sf::Lines);
    
    this->window->draw(line);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    //no converion in cordinates of center and upper left corner, Circle in sfml is managed by default with the center
    sf::CircleShape circle(radius*RATIO);
    circle.setPosition(center.x*RATIO - radius*RATIO, center.y*RATIO - radius*RATIO);
    circle.setFillColor(this->B2SFColor(color, 50));
    circle.setOutlineColor(this->B2SFColor(color));
    circle.setOutlineThickness(1.f);

    // line of the circle wich shows the angle
    b2Vec2 p = center + (radius * axis);
    this->DrawSegment(center, p, color);
    
    this->window->draw(circle);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    sf::CircleShape circle((radius*RATIO));
    circle.setPosition(center.x*RATIO-radius*RATIO, center.y*RATIO-radius*RATIO);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(this->B2SFColor(color));
    circle.setOutlineThickness(1.f);
    this->window->draw(circle);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape polygon(vertexCount);
	for (int32 i=0; i<vertexCount; i++)
	{
		b2Vec2 vertex = vertices[i];
		polygon.setPoint(i, sf::Vector2f(vertex.x*RATIO, vertex.y*RATIO));
	}
    polygon.setFillColor(this->B2SFColor(color, 50));
    polygon.setOutlineColor(this->B2SFColor(color));
	polygon.setOutlineThickness(1.0f);
	this->window->draw(polygon);
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape polygon(vertexCount);
	for (int32 i=0; i<vertexCount; i++)
	{
		b2Vec2 vertex = vertices[i];
		polygon.setPoint(i, sf::Vector2f(vertex.x*RATIO, vertex.y*RATIO));
	}
    polygon.setFillColor(sf::Color::Transparent);
    polygon.setOutlineColor(this->B2SFColor(color));
	polygon.setOutlineThickness(1.0f);
	this->window->draw(polygon);
}
