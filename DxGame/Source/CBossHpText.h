#pragma once
#include "CUIObject.h"

/***********************************************
*			�{�XHp�e�L�X�g
**/


class CBossHpText
	: public CUIObject
	, public SingletonManager<CBossHpText>
{
private:
	CBossHpText();
	~CBossHpText() override;
public:
	friend class SingletonManager<CBossHpText>;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw();


protected:

};




