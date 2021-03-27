#include "SpriteManager.h"
#include "CollisionChecker.h"

SpriteManager SpriteManager::manager;

bool SortUsingZorder(const Sprite* s1, const Sprite* s2) 
{
	return s1->GetZorder() < s2->GetZorder();
}

void SpriteManager::CleanUp()
{
	for (auto sprite : this->deadSprites) 
	{
		delete sprite;
		sprite = nullptr;
	}
	this->deadSprites.clear();

	for (auto sprite : this->displayList)
	{
		delete sprite;
		sprite = nullptr;
	}
	this->displayList.clear();
	this->typeLists.clear();
}

SpriteManager::~SpriteManager()
{
	this->CleanUp();
}

void SpriteManager::Add(Sprite* sprite)
{
	assert(sprite);

	this->displayList.push_front(sprite);
	this->displayList.sort(SortUsingZorder);

	this->typeLists[sprite->GetTypeId()].push_front(sprite);
	this->typeLists[sprite->GetTypeId()].sort(SortUsingZorder);
}

void SpriteManager::Remove(Sprite* sprite)
{
	assert(sprite);

	auto dexist = std::find(this->displayList.begin(), this->displayList.end(), sprite);

	auto& type = sprite->GetTypeId();

	if (type.empty())
		return;

	if (this->typeLists.find(type) != this->typeLists.end())
	{
		auto& list  = this->typeLists.at(type);
		auto  exist = std::find(list.begin(), list.end(), sprite);

		if (exist != list.end()) 
		{
			list.erase(exist);
			list.sort(SortUsingZorder);
		}
	}

	if (dexist != this->displayList.end())
	{
		this->displayList.erase(dexist);
		this->displayList.sort(SortUsingZorder);
	}
}

void SpriteManager::MarkAsDead(Sprite* sprite)
{
	assert(sprite);

	//Remove the sprites from the lists(display , type)
	//and after that "mark it"
	this->Remove(sprite);

	this->deadSprites.push_back(sprite);
}

void SpriteManager::CommitDestruction()
{
	//Remove all the collision pairs with the dead sprites
	//and after that delete the srite and clear the dead vector
	for (auto dead : this->deadSprites) 
	{
		CollisionChecker::GetSigleton().Cancel(dead);

		//we make it null to make it easy to debug
		delete dead;
		dead = nullptr;
	}
	this->deadSprites.clear();
}

bool SpriteManager::TypeExist(const std::string& type) const
{
	return (this->typeLists.find(type) != this->typeLists.end()) ? true : false;
}

void SpriteManager::Draw(Bitmap at)
{
	TileLayer& actionLayer = Terrain::GetActionLayer();

	for (auto sprite : this->displayList)
		if (sprite->GetTypeId().find("mario") == std::string::npos && sprite->GetVisibility())
			sprite->Display(at, actionLayer.GetViewWindow(), Clipper().SetView([&actionLayer]() { return actionLayer.GetViewWindow(); }));
}

const SpriteList& SpriteManager::GetDisplayList() const
{
	return this->displayList;
}

const SpriteList& SpriteManager::GetTypeList(const std::string& typeId) const
{
	return this->typeLists.at(typeId);
}

SpriteManager& SpriteManager::GetSigleton()
{
	return manager;
}
