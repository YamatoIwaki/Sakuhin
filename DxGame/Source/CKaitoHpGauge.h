#pragma once
#include "CUIObject.h"

/********************************************
*		カイトの体力ゲージ
**/

class CKaitoHpGauge
	: public CUIObject
{
public: 
	static constexpr float HpPosX = WND_W / 2 - 150.0f;		//場所X
	static constexpr float HpPosY = WND_H - 100.0f;		//場所Y


public:
	CKaitoHpGauge() {};
	virtual ~CKaitoHpGauge() override {};

	virtual void Init() override {};
	virtual void Update() override {};
	virtual void Draw() override {};


protected:

};

