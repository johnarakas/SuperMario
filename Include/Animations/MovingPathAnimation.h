#ifndef MOVING_PATH_ANIMATION_H
#define MOVING_PATH_ANIMATION_H

#include "Animation.h"

#include <vector>

struct PathEntry
{
	int dx = 0;
	int dy = 0;

	unsigned frame = 0;
	unsigned delay = 0;
	
	PathEntry(void)				= default;
	PathEntry(const PathEntry&) = default;
	
	PathEntry(int _dx, int _dy, unsigned _frame, unsigned _delay) 
	{
		dx	  = _dx;
		dy	  = _dy;
		frame = _frame;
		delay = _delay;
	}
};

using Path = std::vector<PathEntry>;

class MovingPathAnimation: public Animation
{
	private:
		
		Path path;

		unsigned reps = 1; //0 == forever
	
	public:
		
		const Path& GetPath() const;
		void SetPath(const Path& _path);

		bool IsForever();
		void SetForever();

		void SetReps(unsigned _reps);
		unsigned GetReps() const;

		Animation*Clone() const override;

		MovingPathAnimation(const std::string& _id, const Path& _path);
		MovingPathAnimation(const std::string& _id, const Path& _path , unsigned _reps);
};

#endif // !MOVING_PATH_ANIMATION_H

