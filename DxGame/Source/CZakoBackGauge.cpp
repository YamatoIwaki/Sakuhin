#include "CZakoBackGauge.h"

CZakoBackGauge::CZakoBackGauge()
{
}

CZakoBackGauge::~CZakoBackGauge()
{
}

void CZakoBackGauge::Init()
{
}

void CZakoBackGauge::Update()
{
}

void CZakoBackGauge::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	if (m_ZakoHp >= 0.1f)
	{
		m_pSprite->SetPatternNo(0, 0);
		m_pSprite->SetBillboard(true);
		CSpriteObject::Draw(View, Proj);
		m_pSprite->SetBillboard(false);
	}
}
