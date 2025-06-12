#include "CKaitoGreenGauge.h"
#include "CKaito.h"

CKaitoGreenGauge::CKaitoGreenGauge()
{
	//調整
	m_vScale.y	  = 1.005f;
	m_PatternNo.y = 2;	//緑ゲージ
	m_vPosition.y = HpPosY;
	m_vPosition.x = HpPosX + 6.0f;

}

CKaitoGreenGauge::~CKaitoGreenGauge()
{
}

void CKaitoGreenGauge::Init()
{
}

void CKaitoGreenGauge::Update()
{
	//カイトの体力を緑ゲージへと変換する
	m_vScale.x = CKaito::GetInstance()->GetHp() / CKaito::MaxHp;
}

void CKaitoGreenGauge::Draw()
{
	CUIObject::Draw();
}
