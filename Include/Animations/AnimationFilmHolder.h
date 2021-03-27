#ifndef ANIMATION_FILM_HOLDER_H
#define ANIMATION_FILM_HOLDER_H

#include "AnimationFilm.h"
#include "BitmapLoader.h"

#include <map>
#include <fstream>

using Films = std::map<std::string, AnimationFilm*>;

class AnimationFilmHolder
{
	private:
		
		Films films;
	
		static AnimationFilmHolder holder;
	
	private:

		void CleanUp();
		
		AnimationFilmHolder() = default;
	
		~AnimationFilmHolder();
	
	public:

		static AnimationFilmHolder& GetSigleton();
		
		AnimationFilm* GetFilm(const std::string& film) const;

		void ParseEntry(std::ifstream& file, int number_of_boxes, std::string& id, std::vector<Rect>& rects);

		void LoadAll(const std::string& path);
};

#define GETFILM(film) (AnimationFilmHolder::GetSigleton().GetFilm(film))

#endif // !ANIMATION_FILM_HOLDER_H

