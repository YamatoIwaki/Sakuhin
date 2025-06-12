#pragma once

#include "CUIObject.h"

/********************************************
*		�t�F�[�Y�̃e�L�X�g
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

