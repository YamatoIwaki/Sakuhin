#pragma once
#include "CUIObject.h"

/***********************************************
*			ボスHpテキスト
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




