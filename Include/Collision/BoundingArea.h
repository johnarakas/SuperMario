#ifndef BOUNDING_AREA_H
#define BOUNDING_AREA_H

class BoundingBox;
class BoundingCircle;
class BoundingPolygon;

class BoundingArea 
{
	public:

		virtual bool Intersects(const BoundingBox& box) const       = 0;
		virtual bool Intersects(const BoundingCircle& circle) const = 0;
		virtual bool Intersects(const BoundingPolygon& poly) const  = 0;
	
	public:

		virtual bool In(unsigned x, unsigned y) const = 0;
		virtual bool Intersects(const BoundingArea& area) const = 0;
		virtual ~BoundingArea() {};
};

#endif // !BOUNDING_AREA_H

