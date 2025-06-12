#include "CZako2RedGauge.h"

CZako2RedGauge::CZako2RedGauge()
{
}

CZako2RedGauge::~CZako2RedGauge()
{
}

void CZako2RedGauge::Init()
{
}

void CZako2RedGauge::Update()
{
	m_vScale.x = m_Zako2Hp / CZako2::MaxHp;
	m_vScale.z = m_Zako2Hp / CZako2::MaxHp;
}

void CZako2RedGauge::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	if (m_Zako2Hp >= 0.1f)
	{
		m_pSprite->SetPatternNo(0, 1);
		m_pSprite->SetBillboard(true);
		CSpriteObject::Draw(View, Proj);
		m_pSprite->SetBillboard(false);
	}
}
