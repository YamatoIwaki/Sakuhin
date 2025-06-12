#pragma once

#include "CUIObject.h"

/********************************************
*		カイトのHpテキスト
**/

class CKaitoHpText
	: public CUIObject
	, public SingletonManager<CKaitoHpText>
{
private:
	CKaitoHpText();
	~CKaitoHpText() override;
public:
	friend class SingletonManager<CKaitoHpText>;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw();


protected:

};

