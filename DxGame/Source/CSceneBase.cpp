#include "CSceneBase.h"

CSceneBase::CSceneBase()
	: m_pDx11						( nullptr )
	, m_pDx9							( nullptr )

	, m_pGround						( nullptr )

	, m_Zako							()
	, m_Zako2							()
	, m_pKaito							( nullptr )
	, m_pBoss							( nullptr )
{
}

CSceneBase::~CSceneBase()
{
}

void CSceneBase::CreateZako(float x, float y, float z, enScene scene)
{
	m_Zako.push_back(new CZako());

	m_Zako[m_Zako.size() - 1]->SetPosition(D3DXVECTOR3(x, y, z));


	m_Zako[m_Zako.size() - 1]->AttachMesh(*CSkinManager::GetMesh(CSkinManager::Zako));

	m_Zako[m_Zako.size() - 1]->SetScene(scene);

}

void CSceneBase::CreateZako2(float x, float y, float z, enScene scene)
{
	m_Zako2.push_back(new CZako2());

	m_Zako2[m_Zako2.size() - 1]->AttachMesh(*CSkinManager::GetMesh(CSkinManager::Zako2));
	m_Zako2[m_Zako2.size() - 1]->SetPosition(D3DXVECTOR3(x, y, z));

	m_Zako2[m_Zako2.size() - 1]->SetScene(scene);
}
