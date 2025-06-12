#pragma once
#include "CUIObject.h"

/***********************************************
*			fade—pƒuƒ‰ƒbƒN”wŒi
**/


class CFadeBlack
	: public CUIObject
	, public SingletonManager<CFadeBlack>
{
public:
	enum class FadeState 
	{
		None,
		FadingIn,
		FadingOut
	};

private:
	CFadeBlack();
public:

	~CFadeBlack() override;
	friend class SingletonManager<CFadeBlack>;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw();

public:
	void StartFadeOut(float duration);
	void StartFadeIn(float duration);
	bool IsFading() const {
		return m_State != FadeState::None;
	}


protected:

	float		m_Duration;
	float		m_Elapsed;
	FadeState	m_State;

};




