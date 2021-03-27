#include "AnimationFilm.h"

Byte AnimationFilm::GetTotalFrames() const
{
	return this->boxes.size();
}

Bitmap AnimationFilm::GetBitmap() const
{ 
	return this->bitmap; 
}

const std::string& AnimationFilm::GetId() const
{ 
	return this->id; 
}

const Rect& AnimationFilm::GetFrameBox(Byte frameNo) const
{
	assert(this->boxes.size() > frameNo); 
	
	return this->boxes[frameNo];
}

void AnimationFilm::DisplayFrame(Bitmap dest, const Point& at, Byte frameNo) const
{
	assert(dest = al_get_target_bitmap());

	Rect bBox = this->GetFrameBox(frameNo);

	al_draw_bitmap_region(this->bitmap, bBox.x , bBox.y , bBox.w , bBox.h ,at.x , at.y , 0);
}

void AnimationFilm::SetBitmap(Bitmap b)
{
	assert(!this->bitmap); 
	
	this->bitmap = b;
}

void AnimationFilm::Append(const Rect& r)
{ 
	this->boxes.push_back(r); 
}

AnimationFilm::AnimationFilm(const std::string& _id)
{
	this->id = _id;
}

AnimationFilm::AnimationFilm(Bitmap _bitmap, const std::vector<Rect>& _boxes, const std::string& _id)
{
	this->bitmap = _bitmap;
	this->boxes  = _boxes;
	this->id	 = _id;
}
