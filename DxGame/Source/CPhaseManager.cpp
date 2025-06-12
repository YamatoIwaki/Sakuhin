#include "CPhaseManager.h"

CPhaseManager::CPhaseManager()
	: m_Phase()
{
}

CPhaseManager::~CPhaseManager()
{

	//�S�Ẵt�F�[�Y���I�����Ă���폜.
	while (!m_Phase.empty())
	{
		m_Phase.top()->Exit();	//�t�F�[�Y�I������
		m_Phase.pop();			//�t�F�[�Y�̍폜
	}

}


void CPhaseManager::Update()
{
	if (!m_Phase.empty())
	{
		m_Phase.top()->Update();	//���݂̃t�F�[�Y���A�b�v�f�[�g
	}
}

void CPhaseManager::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if (!m_Phase.empty())
	{
		m_Phase.top()->Draw(View, Proj, Light, Camera);		//���݂̃t�F�[�Y��`��
	}

}

void CPhaseManager::DrawShadow()
{
	if (!m_Phase.empty())
	{
		m_Phase.top()->DrawShadow();		//���݂̃t�F�[�Y��`��
	}
}

CPhaseBase* CPhaseManager::GetCurrentPhase()
{
	if (!m_Phase.empty())
	{
		return m_Phase.top().get();
	}
	return nullptr;
}

void CPhaseManager::PushPhase(std::unique_ptr<CPhaseBase> newPhase)
{
	if (!m_Phase.empty())
	{
		m_Phase.top()->Exit();	//���݂̃t�F�[�Y�̏I������
	}

	m_Phase.push(std::move(newPhase));
	m_Phase.top()->Init();	//�V�����t�F�[�Y��������
}

void CPhaseManager::PopPhase()
{
	if (!m_Phase.empty())
	{
		m_Phase.top()->Exit();	//���݂̃t�F�[�Y�̏I������
		m_Phase.pop();			//���݂̃t�F�[�Y���폜
		if (!m_Phase.empty())
		{
			m_Phase.top()->Init();	//���̃t�F�[�Y��������
		}
	}
}

void CPhaseManager::ChangePhase(std::unique_ptr<CPhaseBase> newPhase)
{
	if (!m_Phase.empty())
	{
		m_Phase.top()->Exit();	//���݂̃t�F�[�Y���I��
		m_Phase.pop();			//���݂̃t�F�[�Y���폜
	}

	m_Phase.push(std::move(newPhase));	//�V�����t�F�[�Y���X�^�b�N����
	m_Phase.top()->Init();		//�V�����t�F�[�Y��������

}

