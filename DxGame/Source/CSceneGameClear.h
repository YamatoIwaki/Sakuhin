#pragma once
#include "CSceneBase.h"
#include "CSceneManager.h"

//���̃N���X�ȊO�̃V�[���̃w�b�_�[���C���N���[�h
#include "CSceneTitle.h"

/**************************
*		�Q�[���I�[�o�[�V�[���N���X
**/

class CSceneGameClear
	: public CSceneBase
{
public:
	CSceneGameClear();
	~CSceneGameClear() override;

	void Init() override;

	void Update() override;

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void Exit() override;

private:
	CSceneManager*	m_pSceneManager;

	std::unique_ptr<CUIObject>			m_pGameClearText;

};

