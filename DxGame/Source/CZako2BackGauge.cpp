#include "CZako2BackGauge.h"

CZako2BackGauge::CZako2BackGauge()
{
}

CZako2BackGauge::~CZako2BackGauge()
{
}

void CZako2BackGauge::Init()
{
}

void CZako2BackGauge::Update()
{

}

void CZako2BackGauge::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	if (m_Zako2Hp >= 0.1f)
	{
		m_pSprite->SetPatternNo(0, 0);
		m_pSprite->SetBillboard(true);
		CSpriteObject::Draw(View, Proj);
		m_pSprite->SetBillboard(false);
	}
}
