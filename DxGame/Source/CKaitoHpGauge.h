#pragma once
#include "CUIObject.h"

/********************************************
*		�J�C�g�̗̑̓Q�[�W
**/

class CKaitoHpGauge
	: public CUIObject
{
public: 
	static constexpr float HpPosX = WND_W / 2 - 150.0f;		//�ꏊX
	static constexpr float HpPosY = WND_H - 100.0f;		//�ꏊY


public:
	CKaitoHpGauge() {};
	virtual ~CKaitoHpGauge() override {};

	virtual void Init() override {};
	virtual void Update() override {};
	virtual void Draw() override {};


protected:

};

