#include "CSceneGameOver.h"

#include "CShadowMap.h"

CSceneGameOver::CSceneGameOver()
	: m_pSceneManager		(nullptr)
	, m_pGameOverText		(nullptr)
{
	m_pDx11 = CDirectX11::GetInstance();

	m_pSceneManager = CSceneManager::GetInstance();
	
	//インスタンス生成
	m_pGameOverText = std::make_unique<CUIObject>();

	//画像を設定
	m_pGameOverText->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::enList::GameOverText));

	//位置の設定
	m_pGameOverText->SetPosition(WND_W / 2 - 400.0f, 100.0f, 0.0f);

	m_pStage = std::make_unique<CApperGround>();

	m_pStage->AttachMesh(*CStaticMeshManager::GetMesh(CStaticMeshManager::Stage));

	CreateZako(8.0f, 0.0f, 0.0f, enScene::GameOver);
	CreateZako(-8.0f, 0.0f, 0.0f, enScene::GameOver);
	CreateZako(0.0f, 0.0f, 8.0f, enScene::GameOver);
	CreateZako(0.0f, 0.0f, -8.0f, enScene::GameOver);

	CreateZako2(4.0f, 0.0f, 0.0f, enScene::GameOver);
	CreateZako2(-4.0f, 0.0f, 0.0f, enScene::GameOver);
	CreateZako2(0.0f, 0.0f, 4.0f, enScene::GameOver);
	CreateZako2(0.0f, 0.0f, -4.0f, enScene::GameOver);

}

CSceneGameOver::~CSceneGameOver()
{
}

void CSceneGameOver::Init()
{
	CKaito::GetInstance()->SetScene(enScene::GameOver);
	CBoss::GetInstance()->SetScene(enScene::GameOver);

	CClickText::GetInstance()->Init();

	if (GetAsyncKeyState(VK_LEFT))
	{
	}
}

void CSceneGameOver::Update()
{
	CSoundManager::PlayLoop(CSoundManager::BGM_GameOver);

	CCamera::GetInstance()->Update(enScene::GameOver);

	CLightCamera::GetInstance()->Update();

	CKaito::GetInstance()->Update();

	for (auto z : m_Zako)
	{
		z->Update();
	}

	for (auto z2 : m_Zako2)
	{
		z2->Update();
	}

	CClickText::GetInstance()->Update();

	if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
	{
		CSoundManager::PlaySE(CSoundManager::SE_Click);
		m_pSceneManager->ChangeSceneWithFade(std::make_unique<CSceneTitle>());
	}

}

void CSceneGameOver::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	/////////シャドウマップへの描画/////////
	CShadowMap::GetInstance()->BeginShadow();

	CKaito::GetInstance()->DrawShadowMap();

	CBoss::GetInstance()->DrawShadowMap();

	for (auto z : m_Zako)
	{
		z->DrawShadowMap();
	}

	for (auto z2 : m_Zako2)
	{
		z2->DrawShadowMap();
	}

	CShadowMap::GetInstance()->EndShadow();
	////////////////////////////////////////////


	CKaito::GetInstance()->Draw(View, Proj, Light, Camera);

	for (auto z : m_Zako)
	{
		z->Draw(View, Proj, Light, Camera);
	}

	for (auto z2 : m_Zako2)
	{
		z2->Draw(View, Proj, Light, Camera);
	}


	m_pStage->Draw(View, Proj, Light, Camera);

	m_pDx11->SetDepth(false);
	CClickText::GetInstance()->Draw();
	m_pGameOverText->Draw();
	m_pDx11->SetDepth(true);
}

void CSceneGameOver::Exit()
{
	CSoundManager::Stop(CSoundManager::BGM_GameOver);
}
