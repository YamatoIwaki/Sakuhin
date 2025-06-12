#pragma once
#include "CUIObject.h"

/********************************************
*		ボスの体力ゲージ
**/

class CBossHpGauge
	: public CUIObject
{
public:
	static constexpr float HpPosX = WND_W / 2 - 300.0f;		//場所X
	static constexpr float HpPosY = 20.0f;					//場所Y


public:
	CBossHpGauge() {};
	virtual ~CBossHpGauge() override {};

	virtual void Init() override {};
	virtual void Update() override {};
	virtual void Draw() override {};


protected:

};

