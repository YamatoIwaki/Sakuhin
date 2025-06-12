#include "CKaitoRedGauge.h"
#include "CKaitoGreenGauge.h"
#include "CKaito.h"

CKaitoRedGauge::CKaitoRedGauge()
	: m_LateTime	(LateTime)
{
	//����
	m_vScale.y = 1.005f;
	m_PatternNo.y = 1;	//�ԃQ�[�W
	m_vPosition.y = HpPosY;
	m_vPosition.x = HpPosX + 6.0f;

}

CKaitoRedGauge::~CKaitoRedGauge()
{
}

void CKaitoRedGauge::Init()
{
	m_vScale.x = 1.0f;


}

void CKaitoRedGauge::Update()
{
	//�΃Q�[�W�̑傫��
	float nowGreenGauge = CKaitoGreenGauge::GetInstance()->GetScale().x;

	if (nowGreenGauge < m_vScale.x)
	{
		//�_���[�W���󂯂���
		m_LateTime--;
		if (m_LateTime < 0)
		{
			//���΂炭�o�ƌ��炷
			m_vScale.x -= ReduceGauge;
			if (nowGreenGauge >= m_vScale.x)
			{
				m_LateTime = LateTime;
				m_vScale.x = nowGreenGauge;
			}
		}
	}
	if (nowGreenGauge >= m_vScale.x)
	{
		m_vScale.x = nowGreenGauge;
	}


}

void CKaitoRedGauge::Draw()
{
	CUIObject::Draw();
}
