#include "CBossHpCurrentGauge.h"
#include "CBoss.h"

CBossHpCurrentGauge::CBossHpCurrentGauge()
{
	//����
	m_PatternNo.y = 1;	//�΃Q�[�W
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
	//�J�C�g�̗̑͂�΃Q�[�W�ւƕϊ�����
	m_vScale.x = CBoss::GetInstance()->GetHp() / CBoss::MaxHp;

}

void CBossHpCurrentGauge::Draw()
{
	CUIObject::Draw();
}
