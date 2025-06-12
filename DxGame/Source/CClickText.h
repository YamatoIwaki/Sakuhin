#pragma once

#include "CUIObject.h"

/********************************************
*		クリックのテキスト
**/

class CClickText
	: public CUIObject
	, public SingletonManager<CClickText>
{
public:
	static constexpr float PlaceAlpha = 0.006f;


private:
	CClickText();
public:
	friend class SingletonManager<CClickText>;

	~CClickText() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw();


protected:
	
	float	m_PlaceAlpah;

};

