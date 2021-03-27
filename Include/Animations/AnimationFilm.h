#ifndef ANIMATION_FILM_H
#define ANIMATION_FILM_H

#include "Shapes.h"
#include "Utilities.h"

#include <string>
#include <vector>

class AnimationFilm 
{
	private:

		std::vector<Rect> boxes;
		
		Bitmap bitmap = nullptr;
		
		std::string id;
	
	public:

		Byte GetTotalFrames(void) const;

		Bitmap GetBitmap(void) const;
		
		const std::string& GetId(void) const;
		
		const Rect& GetFrameBox(Byte frameNo) const;
		
		void DisplayFrame(Bitmap dest, const Point& at, Byte frameNo) const;
		
		void SetBitmap(Bitmap b);
		
		void Append(const Rect& r);

		AnimationFilm(const std::string& _id);
		
		AnimationFilm(Bitmap bitmap, const std::vector<Rect>&, const std::string&);
};

#endif // !ANIMATION_FILM_H

