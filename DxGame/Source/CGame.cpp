#include "CGame.h"
#include "CSoundManager.h"
#include "CEffect.h"		//Effekseer���g�����߂̃N���X.

#include "CSceneTitle.h"

//�R���X�g���N�^.
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


//�f�X�g���N�^.
CGame::~CGame()
{
	//�O���ō쐬���Ă���̂ŁA�����Ŕj�����Ȃ�.
	m_hWnd = nullptr;
	m_pDx11 = nullptr;
	m_pDx9 = nullptr;
}

//�\�z.
void CGame::Create()
{
	//�V�[���}�l�[�W���[�̃C���X�^���X�擾
	m_pSceneManager = CSceneManager::GetInstance();

	//�f�o�b�O�e�L�X�g�̃C���X�^���X�쐬.
	m_pDbgText	= CDebugText::GetInstance();

	m_pCamera			= CCamera::GetInstance();

	m_pLightCam			= CLightCamera::GetInstance();

	//�n�ʃN���X�̃C���X�^���X�쐬.
	m_pGround = CGround::GetInstance();
	m_pGround->SetSkinPlayer(*CKaito::GetInstance());
	m_pGround->SetBoss(*CBoss::GetInstance());

	//Effect�N���X.
	CEffect::GetInstance()->Create(m_pDx11->GetDevice(), m_pDx11->GetContext());
}

//���[�h�f�[�^�֐�.
HRESULT CGame::LoadData()
{
	//�X�v���C�g2D�f�[�^�̓ǂݍ���.
	if (CSprite2DManager::GetInstance()->Load() == false) {
		return E_FAIL;
	}


	//�f�o�b�O�e�L�X�g�̓ǂݍ���.
	if ( FAILED( m_pDbgText->Init( *m_pDx11 ) ) ) {
		return E_FAIL;
	}
	

	//�T�E���h�f�[�^�̓ǂݍ���.
	if( CSoundManager::GetInstance()->Load( m_hWnd ) == false ){
		return E_FAIL;
	}

	//Effect�N���X.
	if (FAILED(CEffect::GetInstance()->LoadData())){
		return E_FAIL;
	}

	//�X�L�����b�V���f�[�^�̓ǂݍ���.
	if (CSkinManager::GetInstance()->Load() == false) {
		return E_FAIL;
	}

	//�X�^�e�B�b�N�f�[�^�̓ǂݍ���.
	if (CStaticMeshManager::GetInstance()->Load() == false) {
		return E_FAIL;
	}

	//�X�v���C�g3D�f�[�^�̓ǂݍ���.
	if (CSprite3DManager::GetInstance()->Load() == false) {
		return E_FAIL;
	}

	if (FAILED(CShadowMap::GetInstance()->Init())) {
		return E_FAIL;
	}

	if (FAILED(CConcentLine::GetInstance()->Init())) {
		return E_FAIL;
	}

	//�X�^�e�B�b�N���b�V����ݒ�.
	m_pGround->AttachMesh(*CStaticMeshManager::GetMesh(CStaticMeshManager::RangeOfStage));

	//�X�L�����b�V����ݒ�.
	CKaito::GetInstance()->AttachMesh(*CSkinManager::GetMesh(CSkinManager::enList::Kaito));
	CBoss::GetInstance()->AttachMesh(*CSkinManager::GetMesh(CSkinManager::enList::Boss));

	//UI�摜��ݒ�
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

//����֐�.
void CGame::Release()
{
}


//�X�V����.
void CGame::Update()
{

	m_pCamera->SethWnd(m_hWnd);

	m_pSceneManager->Update();

}

//�`�揈��.
void CGame::Draw()
{

	m_mProj = m_pCamera->GetProjMatrix();
	m_mView = m_pCamera->GetViewMatrix();

	m_pCamera->Draw();

	CAMERA camera = m_pCamera->GetCamera();

	m_pLightCam->Draw(m_mView, m_mProj, camera);

	m_pSceneManager->Draw(m_mView, m_mProj, m_pLightCam->GetLight(), camera);

	//Effect�N���X.
	CEffect::GetInstance()->Draw(m_mView, m_mProj, m_pLightCam->GetLight(), camera);


}

