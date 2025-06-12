#include "CBossHpText.h"

CBossHpText::CBossHpText()
{
	m_vScale *= 1.2f;
	m_vPosition.x = WND_W / 2 - 100.0f;

}


CBossHpText::~CBossHpText()
{
}

void CBossHpText::Init()
{
}

void CBossHpText::Update()
{
}

void CBossHpText::Draw()
{
	CUIObject::Draw();
}
