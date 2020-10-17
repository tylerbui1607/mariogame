#pragma once
#include "Brick.h"
class QuestionBrick :
	public CBrick
{
public:
	virtual void Render();
	void SetState(int state);
};

