#include "CPhase1.h"

CPhase1::CPhase1()
{
	m_pPhaseText = std::make_unique<CPhaseTextBase>();

	m_pPhaseText->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::Phase1Text));


}

CPhase1::~CPhase1()
{
}

void CPhase1::Init()
{
	CStartText::GetInstance()->Init();

	LoadEnemyPos("Data\\csv\\Phase\\Phase1.csv");

}

void CPhase1::Update()
{
	CSoundManager::PlayLoop(CSoundManager::BGM_GameMain);

	for (auto z : m_Zako)
	{
		z->RayUpdate();
	}
	for (auto z2 : m_Zako2)
	{
		z2->RayUpdate();
	}

	CGround::GetInstance()->Update();
	
	m_pPhaseText->Update();

	//テキストが消えるまで動かないようにする
	if (!m_pPhaseText->IsDrawEnd()) { return; }

	CStartText::GetInstance()->Update();

	ZakoSetUpdate();
	Zako2SetUpdate();


	if (std::all_of(m_IsZakoDie.begin(), m_IsZakoDie.end(), [](bool d) {return d; })
		&& std::all_of(m_IsZako2Die.begin(), m_IsZako2Die.end(), [](bool d) {return d; }))
	{
		//すべての敵が死んだら次のフェーズへ
		CSoundManager::Stop(CSoundManager::BGM_GameMain);
		CPhaseManager::GetInstance()->ChangePhase(std::make_unique<CPhase2>());
	}


}

void CPhase1::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ZakoSetDraw(View, Proj, Light, Camera);
	Zako2SetDraw(View, Proj, Light, Camera);

	CDirectX11::GetInstance()->SetDepth(false);

	if (m_pPhaseText->IsDrawEnd())
	{
		CConcentLine::GetInstance()->Draw();

		CStartText::GetInstance()->Draw();
	}

	m_pPhaseText->Draw();

	CDirectX11::GetInstance()->SetDepth(true);

}

void CPhase1::DrawShadow()
{
	for (int no = 0; no < m_Zako.size(); no++)
	{
		m_Zako[no]->DrawShadowMap();
	}

	for (int no = 0; no < m_Zako2.size(); no++)
	{
		m_Zako2[no]->DrawShadowMap();
	}


}

void CPhase1::Exit()
{
}

