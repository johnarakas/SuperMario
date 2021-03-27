#include "AnimationFilmHolder.h"

AnimationFilmHolder AnimationFilmHolder::holder;

void AnimationFilmHolder::CleanUp()
{
	for (auto& film : this->films)
		delete film.second;
	this->films.clear();
}

AnimationFilmHolder::~AnimationFilmHolder()
{
	this->CleanUp();
}

AnimationFilmHolder& AnimationFilmHolder::GetSigleton()
{
	return holder;
}

AnimationFilm* AnimationFilmHolder::GetFilm(const std::string& id) const
{
	auto film = this->films.find(id);

	if (film == films.end())
		std::cerr << "Unable to find \'" + id + "\'" << std::endl;

	return (film != films.end()) ? (film->second) : (nullptr);
}

void AnimationFilmHolder::ParseEntry(std::ifstream& file , int number_of_boxes, std::string& id, std::vector<Rect>& rects)
{
	file >> id;

	for (auto box = 0; box < number_of_boxes; box++) 
	{
		Dim x, y, w, h;

		file >> x;
		file >> y;
		file >> w;
		file >> h;

		rects.push_back({ x , y , w , h });
	}
}

void AnimationFilmHolder::LoadAll(const std::string& path)
{
	std::ifstream animation_film_file;

	animation_film_file.open(path.c_str(), std::ios::in);

	if (!animation_film_file.is_open())
	{
		std::cerr << "Failed to load file from '" << path << "'.\n";
		return;
	}

	int number_of_films;
	std::string animations_bitmap;

	animation_film_file >> animations_bitmap;
	animation_film_file >> number_of_films;

	Bitmap animation_btm = BitmapLoader::GetSigleton().LoadBitmap(animations_bitmap.c_str());

	if (animation_btm)
	{
		for (auto i = 0; i < number_of_films; i++)
		{
			int				  number_of_boxes;
			std::vector<Rect> boxes;
			std::string		  id;

			animation_film_file >> number_of_boxes;

			this->ParseEntry(animation_film_file , number_of_boxes, id, boxes);

			this->films[id] = new AnimationFilm(animation_btm, boxes, id);
		}
	}
	else 
	{
		std::cerr << "Failed to bitmap file from '" << animations_bitmap << "'.\n";
	}

	animation_film_file.close();
}
