#include "CStartText.h"

CStartText::CStartText()
	: m_IsSE(false)

{
	//初期位置をセット
	m_vPosition.x = WND_W / 2 - 500.0f;
	m_vPosition.y = WND_H / 2 - 125.0f;
}

CStartText::~CStartText()
{
}

void CStartText::Init()
{
	m_IsSE = false;
	m_Alpha = 1.0f;
}

void CStartText::Update()
{
	if (m_IsSE == false)
	{
		//一度だけ鳴らす
		m_IsSE = true;
		CSoundManager::PlaySE(CSoundManager::SE_PhaseStart);

	}

	if (m_Alpha >= 0.0f)
	{
		//０になるまで下げる
		m_Alpha -= 0.05f;
	}

}

void CStartText::Draw()
{
	if (m_Alpha >= 0.0f)
	{
		CUIObject::Draw();
	}
}
