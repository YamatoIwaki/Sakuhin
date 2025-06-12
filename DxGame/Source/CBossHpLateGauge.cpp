#include "CBossHpLateGauge.h"
#include "CBossHpCurrentGauge.h"
#include "CBoss.h"

CBossHpLateGauge::CBossHpLateGauge()
	: m_LateTime(LateTime)
{
	//����
	m_PatternNo.y	= 1;	//�ԃQ�[�W
	m_vPosition.y	= HpPosY;
	m_vPosition.x	= HpPosX + 12.0f;
	m_Alpha			= 0.4f;

}

CBossHpLateGauge::~CBossHpLateGauge()
{
}

void CBossHpLateGauge::Init()
{


}

void CBossHpLateGauge::Update()
{
	//���݂̗̑̓Q�[�W�̑傫��
	float nowCurrentGauge = CBossHpCurrentGauge::GetInstance()->GetScale().x;

	if (nowCurrentGauge < m_vScale.x)
	{
		//�_���[�W���󂯂���
		m_LateTime--;
		if (m_LateTime < 0)
		{
			//���΂炭�o�ƌ��炷
			m_vScale.x -= ReduceGauge;
			if (nowCurrentGauge >= m_vScale.x)
			{
				m_LateTime = LateTime;
				m_vScale.x = nowCurrentGauge;
			}
		}
	}
	if (nowCurrentGauge >= m_vScale.x)
	{
		m_vScale.x = nowCurrentGauge;
	}


}

void CBossHpLateGauge::Draw()
{
	CUIObject::Draw();
}
