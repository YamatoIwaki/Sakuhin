#include "CGame.h"
#include "CSoundManager.h"
#include "CEffect.h"		//Effekseerを使うためのクラス.

#include "CSceneTitle.h"

//コンストラクタ.
CGame::CGame( CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd )
	: m_pDx9				( &pDx9 )
	, m_pDx11				( &pDx11 )
	, m_pDbgText			( nullptr )

	, m_pSceneManager		( nullptr )

	, m_hWnd				( hWnd )
	, m_mView				()
	, m_mProj				()

	, m_pCamera				()
	, m_pLightCam			()

	, m_pGround				( nullptr )

{
}


//デストラクタ.
CGame::~CGame()
{
	//外部で作成しているので、ここで破棄しない.
	m_hWnd = nullptr;
	m_pDx11 = nullptr;
	m_pDx9 = nullptr;
}

//構築.
void CGame::Create()
{
	//シーンマネージャーのインスタンス取得
	m_pSceneManager = CSceneManager::GetInstance();

	//デバッグテキストのインスタンス作成.
	m_pDbgText	= CDebugText::GetInstance();

	m_pCamera			= CCamera::GetInstance();

	m_pLightCam			= CLightCamera::GetInstance();

	//地面クラスのインスタンス作成.
	m_pGround = CGround::GetInstance();
	m_pGround->SetSkinPlayer(*CKaito::GetInstance());
	m_pGround->SetBoss(*CBoss::GetInstance());

	//Effectクラス.
	CEffect::GetInstance()->Create(m_pDx11->GetDevice(), m_pDx11->GetContext());
}

//ロードデータ関数.
HRESULT CGame::LoadData()
{
	//スプライト2Dデータの読み込み.
	if (CSprite2DManager::GetInstance()->Load() == false) {
		return E_FAIL;
	}


	//デバッグテキストの読み込み.
	if ( FAILED( m_pDbgText->Init( *m_pDx11 ) ) ) {
		return E_FAIL;
	}
	

	//サウンドデータの読み込み.
	if( CSoundManager::GetInstance()->Load( m_hWnd ) == false ){
		return E_FAIL;
	}

	//Effectクラス.
	if (FAILED(CEffect::GetInstance()->LoadData())){
		return E_FAIL;
	}

	//スキンメッシュデータの読み込み.
	if (CSkinManager::GetInstance()->Load() == false) {
		return E_FAIL;
	}

	//スタティックデータの読み込み.
	if (CStaticMeshManager::GetInstance()->Load() == false) {
		return E_FAIL;
	}

	//スプライト3Dデータの読み込み.
	if (CSprite3DManager::GetInstance()->Load() == false) {
		return E_FAIL;
	}

	if (FAILED(CShadowMap::GetInstance()->Init())) {
		return E_FAIL;
	}

	if (FAILED(CConcentLine::GetInstance()->Init())) {
		return E_FAIL;
	}

	//スタティックメッシュを設定.
	m_pGround->AttachMesh(*CStaticMeshManager::GetMesh(CStaticMeshManager::RangeOfStage));

	//スキンメッシュを設定.
	CKaito::GetInstance()->AttachMesh(*CSkinManager::GetMesh(CSkinManager::enList::Kaito));
	CBoss::GetInstance()->AttachMesh(*CSkinManager::GetMesh(CSkinManager::enList::Boss));

	//UI画像を設定
	CKaitoGreenGauge::GetInstance()->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::enList::KaitoHp));
	CKaitoRedGauge::GetInstance()->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::enList::KaitoHp));
	CKaitoBackGauge::GetInstance()->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::enList::KaitoHp));
	CKaitoHpText::GetInstance()->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::enList::KaitoHpText));

	CBossHpBackGauge::GetInstance()->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::enList::BossHp));
	CBossHpCurrentGauge::GetInstance()->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::enList::BossHp));
	CBossHpLateGauge::GetInstance()->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::enList::BossHp));
	CBossHpText::GetInstance()->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::enList::BossHpText));

	CFadeBlack::GetInstance()->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::enList::FadeBlack));

	CStartText::GetInstance()->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::StartText));
	CClickText::GetInstance()->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::ClickText));

	m_pSceneManager->ChangeScene(std::make_unique<CSceneTitle>());

	return S_OK;
}

//解放関数.
void CGame::Release()
{
}


//更新処理.
void CGame::Update()
{

	m_pCamera->SethWnd(m_hWnd);

	m_pSceneManager->Update();

}

//描画処理.
void CGame::Draw()
{

	m_mProj = m_pCamera->GetProjMatrix();
	m_mView = m_pCamera->GetViewMatrix();

	m_pCamera->Draw();

	CAMERA camera = m_pCamera->GetCamera();

	m_pLightCam->Draw(m_mView, m_mProj, camera);

	m_pSceneManager->Draw(m_mView, m_mProj, m_pLightCam->GetLight(), camera);

	//Effectクラス.
	CEffect::GetInstance()->Draw(m_mView, m_mProj, m_pLightCam->GetLight(), camera);


}

