#pragma once
#include "CUIObject.h"

/********************************************
*		�{�X�̗̑̓Q�[�W
**/

class CBossHpGauge
	: public CUIObject
{
public:
	static constexpr float HpPosX = WND_W / 2 - 300.0f;		//�ꏊX
	static constexpr float HpPosY = 20.0f;					//�ꏊY


public:
	CBossHpGauge() {};
	virtual ~CBossHpGauge() override {};

	virtual void Init() override {};
	virtual void Update() override {};
	virtual void Draw() override {};


protected:

};

