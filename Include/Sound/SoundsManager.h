#ifndef SOUNDS_MANAGER_H
#define SOUNDS_MANAGER_H

#include <allegro5/allegro_audio.h>

#include <map>
#include <string>
#include <fstream>
#include <iostream>

using SoundsMap		= std::map<std::string, ALLEGRO_SAMPLE* >;
using SoundsMapId	= std::map<std::string, ALLEGRO_SAMPLE_ID* >;

class SoundsManager final 
{
	private:

		SoundsMap soundsMap;

		SoundsMapId soundsMapId;

		static SoundsManager sounds;

	public:

		void LoadAllSounds(std::string path);

		void PlaySound(std::string soundName);

		void StopSound(std::string soundName);

		static SoundsManager& GetSigleton();
};

#define PLAYSOUND(sound) (SoundsManager::GetSigleton().PlaySound(std::string(sound)))
#define STOPSOUND(sound) (SoundsManager::GetSigleton().StopSound(std::string(sound)))

#endif // !SOUNDS_MANAGER_H

