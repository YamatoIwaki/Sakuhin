#include "CSceneGameClear.h"

CSceneGameClear::CSceneGameClear()
	: m_pSceneManager		(nullptr)
	, m_pGameClearText			(nullptr)
{
	m_pDx11 = CDirectX11::GetInstance();

	m_pSceneManager = CSceneManager::GetInstance();
	
	//インスタンス生成
	m_pGameClearText = std::make_unique<CUIObject>();

	//画像を設定
	m_pGameClearText->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::GameClearText));

	//位置の設定
	m_pGameClearText->SetPosition(WND_W / 2 - 400.0f, 100.0f, 0.0f);

	m_pStage = std::make_unique<CApperGround>();

	m_pStage->AttachMesh(*CStaticMeshManager::GetMesh(CStaticMeshManager::Stage));

	CreateZako(1.7f, 2.0f, 0.5f, enScene::GameClear);
	CreateZako(-1.3f, 2.0f, 1.2f, enScene::GameClear);
	CreateZako(-0.8f, 2.0f, 0.4f, enScene::GameClear);
	CreateZako(1.5f, 2.0f, -0.6f, enScene::GameClear);
	CreateZako(1.2f, 2.0f, 0.9f, enScene::GameClear);
	CreateZako(-0.8f, 2.0f, 0.3f, enScene::GameClear);


	CreateZako2(2.0f, 0.5f, 1.0f, enScene::GameClear);
	CreateZako2(-2.0f, 0.5f, 2.0f, enScene::GameClear);
	CreateZako2(2.0f, 0.5f, -2.4f, enScene::GameClear);
	CreateZako2(-2.0f, 0.5f, -1.2f, enScene::GameClear);
	CreateZako2(1.0f, 0.5f, 2.3f, enScene::GameClear);
	CreateZako2(-1.0f, 0.5f, 1.5f, enScene::GameClear);
	CreateZako2(1.0f, 0.5f, -2.6f, enScene::GameClear);
	CreateZako2(-1.0f, 0.5f, -2.1f, enScene::GameClear);


}

CSceneGameClear::~CSceneGameClear()
{
}

void CSceneGameClear::Init()
{
	CKaito::GetInstance()->SetScene(enScene::GameClear);
	CBoss::GetInstance()->SetScene(enScene::GameClear);

	CClickText::GetInstance()->Init();

	if (GetAsyncKeyState(VK_LEFT))
	{
	}

}

void CSceneGameClear::Update()
{
	CSoundManager::PlayLoop(CSoundManager::BGM_GameClear);

	CCamera::GetInstance()->Update(enScene::GameClear);

	CLightCamera::GetInstance()->Update();

	for (auto z : m_Zako)
	{
		z->Update();
	}

	for (auto z2 : m_Zako2)
	{
		z2->Update();
	}

	CKaito::GetInstance()->Update();

	CBoss::GetInstance()->Update();

	CClickText::GetInstance()->Update();

	if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
	{
		CSoundManager::PlaySE(CSoundManager::SE_Click);

		m_pSceneManager->ChangeSceneWithFade(std::make_unique<CSceneTitle>());
	}

}

void CSceneGameClear::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
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

	CBoss::GetInstance()->Draw(View, Proj, Light, Camera);

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

	m_pGameClearText->Draw();
	m_pDx11->SetDepth(true);
}

void CSceneGameClear::Exit()
{
	CSoundManager::Stop(CSoundManager::BGM_GameClear);
}
