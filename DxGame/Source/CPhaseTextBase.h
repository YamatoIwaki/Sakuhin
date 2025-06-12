#pragma once

#include "CUIObject.h"

/********************************************
*		フェーズのテキスト
**/

class CPhaseTextBase
	: public CUIObject
{
public:
	CPhaseTextBase();
	~CPhaseTextBase() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw();

	bool			IsDrawEnd() { return m_IsDrawEnd; }

protected:
	bool			m_IsDrawEnd;

};

