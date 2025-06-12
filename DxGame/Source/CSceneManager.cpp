#include "CSceneManager.h"

CSceneManager::CSceneManager()
	: m_Scene		()
	, m_NextScene	(nullptr)
	, m_IsChangeing	(false)
{
}

CSceneManager::~CSceneManager()
{

	//�S�ẴV�[�����I�����Ă���폜.
	while (!m_Scene.empty())
	{
		m_Scene.pop();			//�V�[���̍폜
	}

}


void CSceneManager::Update()
{
	//fade�X�V
	CFadeBlack::GetInstance()->Update();

	//�t�F�[�h�A�E�g�������Ă���V�[���؂�ւ�
	if (m_IsChangeing && !CFadeBlack::GetInstance()->IsFading() && m_NextScene)
	{
		if (!m_Scene.empty())
		{
			m_Scene.top()->Exit();
			m_Scene.pop();
		}

		m_Scene.push(std::move(m_NextScene));
		m_Scene.top()->Init();

		CFadeBlack::GetInstance()->StartFadeIn(1.0f);	//fade�C���J�n
		m_IsChangeing = false;
	}



	if (!m_Scene.empty())
	{
		m_Scene.top()->Update();	//���݂̃V�[�����A�b�v�f�[�g
	}
}

void CSceneManager::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if (!m_Scene.empty())
	{
		m_Scene.top()->Draw(View, Proj, Light, Camera);		//���݂̃V�[����`��
	}

	CFadeBlack::GetInstance()->Draw();


}

CSceneBase* CSceneManager::GetCurrentScene()
{
	if (!m_Scene.empty())
	{
		return m_Scene.top().get();
	}
	return nullptr;
}

void CSceneManager::PushScene(std::unique_ptr<CSceneBase> newScene)
{
	if (!m_Scene.empty())
	{
		m_Scene.top()->Exit();	//���݂̃V�[���̏I������
	}

	m_Scene.push(std::move(newScene));
	m_Scene.top()->Init();	//�V�����V�[����������
}

void CSceneManager::PopScene()
{
	if (!m_Scene.empty())
	{
		m_Scene.top()->Exit();	//���݂̃V�[���̏I������
		m_Scene.pop();			//���݂̃V�[�����폜
		if (!m_Scene.empty())
		{
			m_Scene.top()->Init();	//���̃V�[����������
		}
	}
}

void CSceneManager::ChangeScene(std::unique_ptr<CSceneBase> newScene)
{
	if (!m_Scene.empty())
	{

		m_Scene.top()->Exit();	//���݂̃V�[�����I��
		m_Scene.pop();			//���݂̃V�[�����폜
	}

	m_Scene.push(std::move(newScene));	//�V�����V�[�����X�^�b�N����
	m_Scene.top()->Init();		//�V�����V�[����������

}

void CSceneManager::ChangeSceneWithFade(std::unique_ptr<CSceneBase> newScene)
{
	if (!m_IsChangeing && !CFadeBlack::GetInstance()->IsFading())
	{
		m_NextScene = std::move(newScene);
		CFadeBlack::GetInstance()->StartFadeOut(2.0f);
		m_IsChangeing = true;
	}

}

