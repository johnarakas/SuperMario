#include "GravityHandler.h"

bool GravityHandler::GetIsGravityAddicted() const
{
	return this->gravityAddicted;
}

void GravityHandler::SetIsGravityAddicted(bool _addicted)
{
	this->gravityAddicted = _addicted;
}

bool GravityHandler::GetIsFalling() const
{
	return this->isFalling;
}

void GravityHandler::SetIsFalling(bool _falling)
{
	this->isFalling = _falling;
}

void GravityHandler::Reset()
{
	this->isFalling = false;
}

void GravityHandler::Check(const Rect& r)
{
	if (this->gravityAddicted) 
	{
		if (this->onSolidGround(r)) 
		{
			if (this->isFalling) 
			{
				this->isFalling = false;
				if(onStopFalling)
					this->onStopFalling();
			}
		}
		else 
		{
			if (!this->isFalling) 
			{
				this->isFalling = true;
				if(onStartFalling)
					this->onStartFalling();
			}
		}
	}
}
