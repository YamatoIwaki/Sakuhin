#include "CSceneTitle.h"

#include "CShadowMap.h"

CSceneTitle::CSceneTitle()
	: m_pSceneManager	(nullptr)
	, m_pTitleText		(nullptr)
{
	m_pDx11 = CDirectX11::GetInstance();

	m_pSceneManager = CSceneManager::GetInstance();

	//インスタンス生成
	m_pTitleText = std::make_unique<CUIObject>();

	//画像を設定
	m_pTitleText->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::enList::TitleText));

	//位置の設定
	m_pTitleText->SetPosition(WND_W / 2 - 400.0f, 100.0f, 0.0f);
	
	m_pStage = std::make_unique<CApperGround>();

	m_pStage->AttachMesh(*CStaticMeshManager::GetMesh(CStaticMeshManager::Stage));

	CreateZako(10.0f, 0.0f, 8.0f);
	CreateZako(-10.0f, 0.0f,8.0f);
	CreateZako(0.0f, 0.0f, 10.0f);
	CreateZako(-8.0f, 0.0f, 15.0f);
	CreateZako(8.0f, 0.0f, 15.0f);

	CreateZako2(-8.0f, 0.0f, 0.0f);
	CreateZako2(8.0f, 0.0f, 0.0f);
	CreateZako2(0.0f, 0.0f, 2.0f);

}

CSceneTitle::~CSceneTitle()
{
}

void CSceneTitle::Init()
{

	CKaito::GetInstance()->SetScene(enScene::Title);
	CBoss::GetInstance()->SetScene(enScene::Title);

	CClickText::GetInstance()->Init();

	if (GetAsyncKeyState(VK_LEFT))
	{	}
}

void CSceneTitle::Update()
{
	//BGMのループ再生.
	CSoundManager::PlayLoop(CSoundManager::BGM_Title);

	CLightCamera::GetInstance()->Update();

	CCamera::GetInstance()->Update(enScene::Title);

	CKaito::GetInstance()->Update();

	CBoss::GetInstance()->Update();

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
		m_pSceneManager->ChangeSceneWithFade(std::make_unique<CSceneMain>());
	}

}

void CSceneTitle::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
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

	m_pStage->Draw(View, Proj, Light, Camera);


	for (auto z : m_Zako)
	{
		z->Draw(View, Proj, Light, Camera);
	}

	for (auto z2 : m_Zako2)
	{
		z2->Draw(View, Proj, Light, Camera);
	}

	m_pDx11->SetDepth(false);
	CClickText::GetInstance()->Draw();
	m_pTitleText->Draw();
	m_pDx11->SetDepth(true);


}

void CSceneTitle::Exit()
{
	CSoundManager::Stop(CSoundManager::BGM_Title);
}

