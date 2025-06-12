#include "CZakoRedGauge.h"

CZakoRedGauge::CZakoRedGauge()
{
}

CZakoRedGauge::~CZakoRedGauge()
{
}

void CZakoRedGauge::Init()
{
}

void CZakoRedGauge::Update()
{
	m_vScale.x = m_ZakoHp / CZako::MaxHp;
	m_vScale.z = m_ZakoHp / CZako::MaxHp;

}

void CZakoRedGauge::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	if (m_ZakoHp >= 0.1f)
	{
		m_pSprite->SetPatternNo(0, 1);
		m_pSprite->SetBillboard(true);
		CSpriteObject::Draw(View, Proj);
		m_pSprite->SetBillboard(false);
	}
}
