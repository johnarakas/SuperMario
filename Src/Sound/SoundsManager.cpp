#include "SoundsManager.h"

SoundsManager SoundsManager::sounds;

void SoundsManager::LoadAllSounds(std::string path)
{
	std::ifstream sounds_file;

	sounds_file.open(path.c_str(), std::ios::in);

	if (!sounds_file.is_open())
	{
		std::cerr << "Failed to load file from '" << path << "'.\n";
		return;
	}

	int number_of_sounds;

	sounds_file >> number_of_sounds;

	if (!al_reserve_samples(number_of_sounds))
	{
		std::cerr << "Failed to reserve " + std::to_string(number_of_sounds) + " samples.\n";
		return;
	}

	for (auto sound = 0; sound < number_of_sounds; sound++)
	{
		std::string soundId;
		std::string soundPath;

		sounds_file >> soundId;
		sounds_file >> soundPath;

		if ((this->soundsMap[soundId] = al_load_sample(soundPath.c_str())) == nullptr)
		{
			std::cerr << "Failed to load the sound sample " + soundId + ".\n";
			return;
		}

		this->soundsMapId[soundId] = new ALLEGRO_SAMPLE_ID;
	}

	sounds_file.close();
}

void SoundsManager::PlaySound(std::string soundName)
{
	auto exist = this->soundsMap.find(soundName);

	if (exist == this->soundsMap.end())
	{
		std::cerr << "Sound sample with name : " + soundName + " is not exist! \n";
		return;
	}

	al_play_sample(exist->second, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, this->soundsMapId[soundName]);
}

void SoundsManager::StopSound(std::string soundName)
{
	auto exist = this->soundsMapId.find(soundName);

	if (exist == this->soundsMapId.end())
	{
		std::cerr << "Sound sample with name : " + soundName + " is not exist! \n";
		return;
	}

	al_stop_sample(exist->second);
}

SoundsManager& SoundsManager::GetSigleton()
{
	return sounds;
}
