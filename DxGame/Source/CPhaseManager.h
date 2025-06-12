#pragma once

#include <stack>
#include <memory>

#include "CPhaseBase.h"
#include "CSingletonManager.h"

class CPhaseManager
	: public SingletonManager<CPhaseManager>
{
public:
	friend class SingletonManager<CPhaseManager>;

	void Update();

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera);

	void DrawShadow();


	//���݂̃V�[�����擾
	CPhaseBase* GetCurrentPhase();

	//�V�[����ǉ�����
	void PushPhase(std::unique_ptr<CPhaseBase> newPhase);

	//�V�[���̍폜
	void PopPhase();

	//�V�[����؂�ւ���
	void ChangePhase(std::unique_ptr<CPhaseBase> newPhase);

protected:
	CPhaseManager();
	~CPhaseManager();

	std::stack<std::unique_ptr<CPhaseBase>> m_Phase;


};

