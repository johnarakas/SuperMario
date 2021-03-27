#include "Sprite.h"
#include "SpriteManager.h"

const Mover Sprite::MakeSpriteMover(GridLayer* layer, Sprite* sprite)
{
	return ([layer, sprite](const Rect& r, int* dx, int* dy)
	{
		layer->FilterGridMotion(r, dx, dy);

		sprite->SetPosition(sprite->GetPositionX() + *dx, sprite->GetPositionY() + *dy);
	});
}

void Sprite::PrepareSpriteGravity(GridLayer* layer, Sprite* sprite)
{
	sprite->GetGravityHandler().SetIsGravityAddicted(true);

	sprite->GetGravityHandler().SetOnSolidGround([layer](const Rect& r)
	{
		return layer->IsOnSolidGround(r);
	});
}

Rect Sprite::GetFrameBox() const
{
	auto viewWindow = Terrain::GetActionLayer().GetViewWindow();

	return {this->x + viewWindow.x , this->y + viewWindow.y, this->frameBox.w , this->frameBox.h};
}

void Sprite::Move(int _dx, int _dy)
{
	if (this->directMotion)
	{
		this->x += _dx;
		this->y += _dy;
	}
	else
	{
		Rect frameBox = this->GetFrameBox();
		
		this->quantizer.Move(frameBox, &_dx, &_dy);
		this->gravity.Check(frameBox);
	}
}

void Sprite::SetPosition(int _x, int _y)
{
	this->x = _x;
	this->y = _y;
}

int Sprite::GetPositionX() const
{
	return this->x;
}

void Sprite::SetPositionX(int _x)
{
	this->x = _x;
}

int Sprite::GetPositionY() const
{
	return this->y;
}

void Sprite::SetPositionY(int _y)
{
	this->y = _y;
}

void Sprite::SetZorder(unsigned int _zOrder)
{
	this->zOrder = _zOrder;
}

unsigned int Sprite::GetZorder() const
{
	return this->zOrder;
}

Byte Sprite::GetFrame() const
{
	return this->frameNo;
}

void Sprite::SetFrame(Byte _frameNo)
{
	if (_frameNo != this->frameNo) 
	{
		assert(_frameNo < currFilm->GetTotalFrames());

		this->frameNo		= _frameNo;
		this->frameBox		= this->currFilm->GetFrameBox(_frameNo);
		
		this->boundingArea	= new BoundingBox(this->x, this->y, this->x + frameBox.w, this->y + frameBox.h);
	}
}

const BoundingArea* Sprite::GetBoundingArea() const
{
	return this->boundingArea;
}

void Sprite::SetBoundingArea(const BoundingArea& _boundingArea)
{
	assert(!this->boundingArea);
}

void Sprite::SetBoundingArea(BoundingArea* _boundingArea)
{
	this->boundingArea = _boundingArea;
}

void Sprite::SetBoundingAreaFromFrameBox()
{
	Rect frameBox = this->GetFrameBox();

	this->boundingArea = new BoundingBox(frameBox.x, frameBox.y, frameBox.x + frameBox.w, frameBox.y + frameBox.h);
}

GravityHandler& Sprite::GetGravityHandler()
{
	return this->gravity;
}

void Sprite::SetHasDirectMotion(bool _directMotion)
{
	this->directMotion = _directMotion;
}

bool Sprite::GetHasDirectMotion()
{
	return this->directMotion;
}

const std::string& Sprite::GetTypeId() const
{
	return this->typeId;
}

void Sprite::SetTypeId(std::string _typeId)
{
	this->typeId = _typeId;
}

const std::string& Sprite::GetStateId() const
{
	return this->stateId;
}

void Sprite::SetStateId(std::string _stateId)
{
	this->stateId = _stateId;
}

bool Sprite::GetVisibility() const
{
	return this->isVisible;
}

void Sprite::SetVisibility(bool _isVisible)
{
	this->isVisible = _isVisible;
}

bool Sprite::CollisionCheck(const Sprite* s) const
{	
	return this->boundingArea->Intersects(dynamic_cast<const BoundingBox&>(*s->GetBoundingArea()));
}

void Sprite::Display(Bitmap dest, const Rect& displayArea, const Clipper& clipper) const
{
	al_draw_bitmap_region(this->currFilm->GetBitmap(), 
						  frameBox.x, 
						  frameBox.y,
						  frameBox.w, 
						  frameBox.h, 
						  this->x - displayArea.x,
						  this->y - displayArea.y,
						  0);
}

void Sprite::Display(Bitmap dest) const
{
	al_draw_bitmap_region(this->currFilm->GetBitmap(),
							frameBox.x,
							frameBox.y,
							frameBox.w,
							frameBox.h,
							this->x,
							this->y,
							0);
}

Sprite::Sprite(AnimationFilm* _currFilm, int _x, int _y, const std::string & _typeId)
{
	this->x			= _x;
	this->y			= _y;
	this->currFilm	= _currFilm;
	this->typeId	= _typeId;
	this->frameNo	= this->currFilm->GetTotalFrames();

	this->SetFrame(0);

	SpriteManager::GetSigleton().Add(this);
}

Sprite::~Sprite()
{
	if (this->boundingArea != nullptr)
	{
		delete this->boundingArea;
		this->boundingArea = nullptr;
	}
}
