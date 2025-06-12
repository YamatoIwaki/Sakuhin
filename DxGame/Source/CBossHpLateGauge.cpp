#include "CBossHpLateGauge.h"
#include "CBossHpCurrentGauge.h"
#include "CBoss.h"

CBossHpLateGauge::CBossHpLateGauge()
	: m_LateTime(LateTime)
{
	//調整
	m_PatternNo.y	= 1;	//赤ゲージ
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
	//現在の体力ゲージの大きさ
	float nowCurrentGauge = CBossHpCurrentGauge::GetInstance()->GetScale().x;

	if (nowCurrentGauge < m_vScale.x)
	{
		//ダメージを受けたら
		m_LateTime--;
		if (m_LateTime < 0)
		{
			//しばらく経つと減らす
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
