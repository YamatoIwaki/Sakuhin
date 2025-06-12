#pragma once

#include "CUIObject.h"

/********************************************
*		スタートのテキスト
**/

class CStartText
	: public CUIObject
	, public SingletonManager<CStartText>
{
private:
	CStartText();
public:
	friend class SingletonManager<CStartText>;

	~CStartText() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw();


protected:
	bool			m_IsSE;


};

