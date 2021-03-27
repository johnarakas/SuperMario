#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "BoundingArea.h"
#include "BoundingCircle.h"
#include "BoundingPolygon.h"

class BoundingBox : public BoundingArea 
{
	protected:

		unsigned x1;
		unsigned y1;
		unsigned x2;
		unsigned y2;
	
	public:

		bool Intersects(const BoundingBox& box) const override;
		bool Intersects(const BoundingCircle& circle) const override;
		bool Intersects(const BoundingPolygon& poly) const override;

		bool In(unsigned x, unsigned y) const override;
		bool Intersects(const BoundingArea& area) const override;

		BoundingBox* Clone() const;

		BoundingBox(unsigned _x1, unsigned _y1, unsigned _x2, unsigned _y2);
};

#endif // !BOUNDING_BOX_H

