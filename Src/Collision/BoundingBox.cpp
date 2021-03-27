#include "BoundingBox.h"

bool BoundingBox::Intersects(const BoundingBox& box) const
{
	auto result = !(box.x2 < x1 ||
					box.x1 > x2 ||
					box.y2 < y1 ||
					box.y1 > y2);
	
	return result;
}

bool BoundingBox::Intersects(const BoundingCircle& circle) const
{
	return circle.Intersects(*this);
}

bool BoundingBox::Intersects(const BoundingPolygon& poly) const
{
	BoundingPolygon::Polygon points;

	points.push_back(Point(x1, y1));
	points.push_back(Point(x2, y2));

	BoundingPolygon selfPolygon(points);

	return poly.Intersects(selfPolygon);
}

bool BoundingBox::In(unsigned x, unsigned y) const
{
	return (x1 <= x && 
			x <= x2 && 
			y1 <= y && 
			y <= y2 );
}

bool BoundingBox::Intersects(const BoundingArea& area) const
{
	return area.Intersects(*this);
}

BoundingBox* BoundingBox::Clone() const
{
	return new BoundingBox(this->x1 , this->y1 , this->x2 , this->y2);
}

BoundingBox::BoundingBox(unsigned _x1, unsigned _y1, unsigned _x2, unsigned _y2)
{
	this->x1 = _x1;
	this->y1 = _y1;
	this->x2 = _x2;
	this->y2 = _y2;
}
