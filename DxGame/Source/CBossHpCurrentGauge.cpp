#include "CBossHpCurrentGauge.h"
#include "CBoss.h"

CBossHpCurrentGauge::CBossHpCurrentGauge()
{
	//調整
	m_PatternNo.y = 1;	//緑ゲージ
	m_vPosition.y = HpPosY;
	m_vPosition.x = HpPosX + 12.0f;

}

CBossHpCurrentGauge::~CBossHpCurrentGauge()
{
}

void CBossHpCurrentGauge::Init()
{
}

void CBossHpCurrentGauge::Update()
{
	//カイトの体力を緑ゲージへと変換する
	m_vScale.x = CBoss::GetInstance()->GetHp() / CBoss::MaxHp;

}

void CBossHpCurrentGauge::Draw()
{
	CUIObject::Draw();
}
