#include "CKaitoGreenGauge.h"
#include "CKaito.h"

CKaitoGreenGauge::CKaitoGreenGauge()
{
	//����
	m_vScale.y	  = 1.005f;
	m_PatternNo.y = 2;	//�΃Q�[�W
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
	//�J�C�g�̗̑͂�΃Q�[�W�ւƕϊ�����
	m_vScale.x = CKaito::GetInstance()->GetHp() / CKaito::MaxHp;
}

void CKaitoGreenGauge::Draw()
{
	CUIObject::Draw();
}
