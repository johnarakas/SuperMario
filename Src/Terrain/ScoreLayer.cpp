#include "ScoreLayer.h"

ScoreLayer ScoreLayer::scoreLayer;

int ScoreLayer::score = 0;
int ScoreLayer::coins = 0;
int ScoreLayer::lives = 3;

void ScoreLayer::DrawLives()
{
	std::string _lives = std::to_string(this->lives);

	if (this->lives >= MAX_LIVES)
		_lives = std::to_string(MAX_LIVES);

	al_draw_text(this->marioFont, FONT_COLOR ,LIVES_X, LIVES_Y, 0, LIVES);

	al_draw_text(this->marioFont, FONT_COLOR ,LIVES_X + OFFESET_X, LIVES_Y + OFFESET_Y, 0, _lives.c_str());
}

void ScoreLayer::DrawScore()
{
	std::string _score = std::to_string(this->score);

	if (this->score >= MAX_SCORE)
		_score = std::to_string(MAX_SCORE);

	al_draw_text(this->marioFont, FONT_COLOR, SCORE_X, SCORE_Y, 0, SCORE);

	al_draw_text(this->marioFont, FONT_COLOR, SCORE_X + OFFESET_X, SCORE_Y + OFFESET_Y, 0, _score.c_str());
}

void ScoreLayer::DrawCoins()
{
	std::string _coins = std::to_string(this->coins);

	if (this->coins >= MAX_COINS)
		_coins = std::to_string(MAX_COINS);

	al_draw_text(this->marioFont, FONT_COLOR, COINS_X, COINS_Y, 0, COINS);

	al_draw_text(this->marioFont, FONT_COLOR, COINS_X + OFFESET_X, COINS_Y + OFFESET_Y, 0, _coins.c_str());
}

void ScoreLayer::SetLives(int _lives)
{
	lives = _lives;
}

void ScoreLayer::SetScore(int _score)
{
	score = _score;
}

void ScoreLayer::SetCoins(int _coins)
{
	coins = _coins;
}

int ScoreLayer::GetLives()
{
	return lives;
}

int ScoreLayer::GetScore()
{
	return score;
}

int ScoreLayer::GetCoins()
{
	return coins;
}

void ScoreLayer::Draw()
{
	this->DrawScore();
	this->DrawCoins();
	this->DrawLives();
}

bool ScoreLayer::InitializeScoreLayer()
{
	if (!(this->marioFont = al_load_ttf_font(FONT_PATH, FONT_SIZE, 0)))
	{
		std::cerr << "Failed to load the base font!\n";
		return false;
	}

	this->lives = 3;
	this->coins = 0;
	this->score = 0;

	return true;
}

void ScoreLayer::Destroy()
{
	al_destroy_font(this->marioFont);
	this->marioFont = nullptr;
}

ScoreLayer& ScoreLayer::GetSigleton()
{
	return scoreLayer;
}
