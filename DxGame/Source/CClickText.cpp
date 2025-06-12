#include "CClickText.h"

CClickText::CClickText()
	: m_PlaceAlpah		(PlaceAlpha)
{
	m_vPosition = D3DXVECTOR3(WND_W / 2 - 400.0f, 300.0f, 0.0f);

}

CClickText::~CClickText()
{
}

void CClickText::Init()
{

	m_Alpha = 1.0f;

}

void CClickText::Update()
{
	//プラスするアルファ値を切り替え
	if (m_Alpha < 0.4f)
	{
		m_PlaceAlpah = PlaceAlpha;
	}
	else if (m_Alpha > 1.0f)
	{
		m_PlaceAlpah = -PlaceAlpha;
	}



	m_Alpha += m_PlaceAlpah;

}

void CClickText::Draw()
{
	CUIObject::Draw();
}
