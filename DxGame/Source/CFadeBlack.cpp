#include "CFadeBlack.h"

CFadeBlack::CFadeBlack()
	: m_Duration	(1.0f)
	, m_Elapsed		(0.0f)
	, m_State		(FadeState::None)
{
	m_Alpha = 0.f;
}

CFadeBlack::~CFadeBlack()
{
}

void CFadeBlack::Init()
{
	
}

void CFadeBlack::Update()
{
	if (m_State == FadeState::None) return;

	m_Elapsed += 1.0f / FPS;
	float t = m_Elapsed / m_Duration;
	t = (t > 1.0f) ? 1.0f : t;

	if (m_State == FadeState::FadingOut)
	{
		m_Alpha = t;
	}
	else if (m_State == FadeState::FadingIn)
	{
		m_Alpha = 1.0f - t;
	}

	if (t >= 1.0f)
	{
		m_State = FadeState::None;
	}


}

void CFadeBlack::Draw()
{
	CUIObject::Draw();
}

void CFadeBlack::StartFadeOut(float duration)
{
	m_State = FadeState::FadingOut;
	m_Alpha = 0.0f;
	m_Elapsed = 0.0f;
	m_Duration = duration;
}

void CFadeBlack::StartFadeIn(float duration)
{
	m_State = FadeState::FadingIn;
	m_Alpha = 1.0f;
	m_Elapsed = 0.0f;
	m_Duration = duration;
}
