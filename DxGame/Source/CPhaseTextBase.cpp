#include "CPhaseTextBase.h"

CPhaseTextBase::CPhaseTextBase()
	: m_IsDrawEnd			(false)
{
	//初期位置をセット
	m_vPosition.x = WND_W / 2 - 300.0f;
	m_vPosition.y = WND_H / 2 - 75.0f;
}

CPhaseTextBase::~CPhaseTextBase()
{
}

void CPhaseTextBase::Init()
{
	m_IsDrawEnd		= false;
	m_Alpha				= 1.0f;
}

void CPhaseTextBase::Update()
{


	if (m_IsDrawEnd == false)
	{
		m_Alpha -= 0.005f;
		if (m_Alpha <= 0.0f)
		{
			CConcentLine::GetInstance()->StartEffect();
			m_IsDrawEnd = true;
		}
	}

}

void CPhaseTextBase::Draw()
{
	CUIObject::Draw();
}
