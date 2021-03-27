#ifndef BOUNDING_POLYGON_H
#define BOUNDING_POLYGON_H

#include "BoundingArea.h"
#include "Shapes.h"

#include <list>

class BoundingPolygon : public BoundingArea 
{
	public:
		using Polygon = std::list<Point>;
	protected:
		Polygon points;
	public:

		virtual bool Intersects(const BoundingBox& box) const override			{ return false; };
		virtual bool Intersects(const BoundingCircle& circle) const override	{ return false; };
		virtual bool Intersects(const BoundingPolygon& poly) const override		{ return false; };

		virtual bool In(unsigned x, unsigned y) const override { return false; };
		virtual bool Intersects(const BoundingArea& area) const override { return false; };

		virtual BoundingPolygon* Clone() const { return nullptr; };
		BoundingPolygon(const Polygon& _points) {};
};

#endif // !BOUNDING_POLYGON_H

