#ifndef BOUNDING_CIRCLE_H
#define BOUNDING_CIRCLE_H

#include "BoundingArea.h"

class BoundingCircle : public BoundingArea 
{
	protected:

		unsigned x;
		unsigned y;
		unsigned r;

	public:

		virtual bool Intersects(const BoundingBox& box) const override		 { return false; };
		virtual bool Intersects(const BoundingCircle& circle) const override { return false; };
		virtual bool Intersects(const BoundingPolygon& poly) const override	 { return false; };

		virtual bool In(unsigned x, unsigned y) const override { return false; };
		virtual bool Intersects(const BoundingArea& area) const override { return false; };
		
		virtual BoundingCircle* Clone() const { return nullptr; };
		BoundingCircle(unsigned _x, unsigned _y, unsigned _r) {};
};

#endif // !BOUNDING_CIRCLE_H

