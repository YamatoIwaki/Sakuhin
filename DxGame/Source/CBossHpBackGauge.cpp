#include "CBossHpBackGauge.h"

CBossHpBackGauge::CBossHpBackGauge()
{

	//����
	m_vPosition.y = HpPosY;
	m_vPosition.x = HpPosX;
}

CBossHpBackGauge::~CBossHpBackGauge()
{
}

void CBossHpBackGauge::Init()
{
}

void CBossHpBackGauge::Update()
{
}

void CBossHpBackGauge::Draw()
{
	CUIObject::Draw();
}
