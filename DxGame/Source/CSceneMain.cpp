#include "CSceneMain.h"

CSceneMain::CSceneMain()
	: m_pSceneManager		(nullptr)
	, m_pUIOpeInstruct		(nullptr)
	, m_pUIOpeInstructSub	(nullptr)
	, m_ChangeSceneTrigger	(false)

{
	m_pDx11 = CDirectX11::GetInstance();

	m_pSceneManager = CSceneManager::GetInstance();

	m_pGround	= CGround::GetInstance();

	m_pStage = std::make_unique<CApperGround>();
	m_pFence = std::make_unique<CApperGround>();

	m_pStage->AttachMesh(*CStaticMeshManager::GetMesh(CStaticMeshManager::Stage));
	m_pFence->AttachMesh(*CStaticMeshManager::GetMesh(CStaticMeshManager::ApperRangeOfStage));

	m_pKaito	= CKaito::GetInstance();

	m_pUIOpeInstruct = std::make_unique<CUIObject>();

	m_pUIOpeInstruct->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::OpeInstruct));

	m_pUIOpeInstructSub = std::make_unique<CUIObject>();

	m_pUIOpeInstructSub->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::OpeInstructSub));

	m_pUIOpeInstructSub->SetPosition(200.0f, 0.0f, 0.0f);
}

CSceneMain::~CSceneMain()
{
}

void CSceneMain::Init()
{
	m_pKaito->Init();
	CBoss::GetInstance()->Init();
	m_pGround->Init();

	CKaitoRedGauge::GetInstance()->Init();

	CPhaseManager::GetInstance()->ChangePhase(std::make_unique<CPhase1>());


}

void CSceneMain::Update()
{
	CCamera::GetInstance()->SetViewPos(m_pKaito->GetPosition());

	CCamera::GetInstance()->Update(enScene::GameMain);

	CLightCamera::GetInstance()->Update();


	m_pKaito->Update();

	CPhaseManager::GetInstance()->Update();

	CKaitoGreenGauge::GetInstance()->Update();
	CKaitoRedGauge::GetInstance()->Update();

	
	if (m_pKaito->GetIsDie() == true && !m_ChangeSceneTrigger)
	{
		m_ChangeSceneTrigger = true;
		//プレイヤーのHpがゼロになったらゲームオーバー
		m_pSceneManager->ChangeSceneWithFade(std::make_unique<CSceneGameOver>());
	}
	else if (CBoss::GetInstance()->GetIsDie() == true && !m_ChangeSceneTrigger)
	{
		m_ChangeSceneTrigger = true;
		//ボスのHpがゼロになったらゲームクリア
		m_pSceneManager->ChangeSceneWithFade(std::make_unique<CSceneGameClear>());
	}


}

void CSceneMain::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{



	/////////シャドウマップへの描画/////////
	CShadowMap::GetInstance()->BeginShadow();

	m_pKaito->DrawShadowMap();

	CPhaseManager::GetInstance()->DrawShadow();

	m_pStage->DrawShadowMap();
	m_pFence->DrawShadowMap();

	CShadowMap::GetInstance()->EndShadow();
	////////////////////////////////////////////


	m_pKaito->Draw(View, Proj, Light, Camera);

	m_pStage->Draw(View, Proj, Light, Camera);
	m_pFence->Draw(View, Proj, Light, Camera);

	CPhaseManager::GetInstance()->Draw(View, Proj, Light, Camera);


	m_pDx11->SetDepth(false);
	CKaitoBackGauge::GetInstance()->Draw();
	CKaitoRedGauge::GetInstance()->Draw();
	CKaitoGreenGauge::GetInstance()->Draw();
	CKaitoHpText::GetInstance()->Draw();

	m_pUIOpeInstruct->Draw();
	m_pUIOpeInstructSub->Draw();

	m_pDx11->SetDepth(true);

}

void CSceneMain::Exit()
{
	CSoundManager::Stop(CSoundManager::BGM_Boss);
	CSoundManager::Stop(CSoundManager::BGM_GameMain);

}

