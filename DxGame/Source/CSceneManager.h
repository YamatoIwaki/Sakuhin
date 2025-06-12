#pragma once

#include <stack>
#include <memory>

#include "CSceneBase.h"
#include "CSingletonManager.h"

class CSceneManager
	: public SingletonManager<CSceneManager>
{
public:
	friend class SingletonManager<CSceneManager>;

	void Update();

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera);

	//���݂̃V�[�����擾
	CSceneBase* GetCurrentScene();

	//�V�[����ǉ�����
	void PushScene(std::unique_ptr<CSceneBase> newScene);

	//�V�[���̍폜
	void PopScene();

	//�V�[����؂�ւ���
	void ChangeScene(std::unique_ptr<CSceneBase> newScene);

	//�V�[����؂�ւ���
	void ChangeSceneWithFade(std::unique_ptr<CSceneBase> newScene);

protected:
	CSceneManager();
	~CSceneManager();

	std::stack<std::unique_ptr<CSceneBase>> m_Scene;
	std::unique_ptr<CSceneBase> m_NextScene = nullptr;

	bool m_IsChangeing = false;
};

