#ifndef GAME_H
#define GAME_H

#include <functional>
#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "SystemTime.h"

using Action = std::function<void(void)>;
using Pred   = std::function<bool(void)>;

extern ALLEGRO_DISPLAY*					display;
extern ALLEGRO_EVENT_QUEUE*				queue;
extern ALLEGRO_TIMER*					timer;
extern ALLEGRO_EVENT					event;
extern ALLEGRO_KEYBOARD_STATE			state;

namespace app
{
	class Game {
		
		private:
			
			Action render, anim, input, ai, physics, destruct, collisions, user;
			Pred   done;

			bool isFinished = false;

			//Pause game variables
			Action pauseResume;
			Action startGame;

			bool isPaused      = false;
			uint64_t pauseTime = 0;

			void Invoke(const Action& f)					{ if (f) f(); }
		
		public:

			bool isStart = true;
			uint64_t  startTime = 0;

			template <typename Tfunc>
			void SetRender(const Tfunc& f)					{ render = f; }
			
			template <typename Tfunc>
			void SetProgressAnimations(const Tfunc& f)		{ anim = f; }
			
			template <typename Tfunc>
			void SetInput(const Tfunc& f)					{ input = f; }
			
			template <typename Tfunc>
			void SetAI(const Tfunc& f)						{ ai = f; }
			
			template <typename Tfunc>
			void SetPhysics(const Tfunc& f)					{ physics = f; }
			
			template <typename Tfunc>
			void SetCollisionChecking(const Tfunc& f)		{ destruct = f; }
			
			template <typename Tfunc>
			void SetCommitDestructions(const Tfunc& f)		{ collisions = f; }
			
			template <typename Tfunc>
			void SetUserCode(const Tfunc& f)				{ user = f; }

			template <typename Tfunc>
			void SetIsFinished(const Tfunc& f)				{ done = f; }

			template<typename Tfunc> 
			void SetOnPauseResume(const Tfunc& f)			{ pauseResume = f; }			
			
			template<typename Tfunc> 
			void SetOnStartGame(const Tfunc& f)				{ startGame = f; }

			void Render(void)								{ Invoke(render); }
			
			void ProgressAnimations(void)					{ Invoke(anim); }
			
			void Input(void)								{ Invoke(input); }
			
			void AI(void)									{ Invoke(ai); }
			
			void Physics(void)								{ Invoke(physics); }
			
			void CollisionChecking(void)					{ Invoke(collisions); }
			
			void CommitDestructions(void)					{ Invoke(destruct); }
			
			void UserCode(void)								{ Invoke(user); }

			void StartGame(void)							{ Invoke(startGame); }
			
			bool IsFinished(void) const						{ 
																if (done) 
																	return done();
																return false;
															}
			void SetIsFinished(bool _isFinished);
			bool GetIsFinished() const;

			void Pause(uint64_t t);
			void Resume();
			
			bool IsPaused() const;
			uint64_t GetPauseTime() const;
			
			void MainLoop(void);
			void MainLoopIteration(void);
	};
}

#endif // !GAME_H