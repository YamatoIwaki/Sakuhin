#include "CKaitoHpText.h"

CKaitoHpText::CKaitoHpText()
{
	m_vScale *= 1.2f;
	m_vPosition.y = WND_H - 105.0f;
	m_vPosition.x = WND_W / 2 - 50.0f;
}


CKaitoHpText::~CKaitoHpText()
{
}

void CKaitoHpText::Init()
{
}

void CKaitoHpText::Update()
{
}

void CKaitoHpText::Draw()
{
	CUIObject::Draw();
}
