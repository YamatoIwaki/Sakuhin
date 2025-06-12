#pragma once
/******************************/
#include "CDirectX9.h"
#include "CDirectX11.h"
#include "CCamera.h"
#include "CLightCamera.h"
#include "CRay.h"
/******************************/
#include "CDebugText.h"
#include "CSprite3D.h"
#include "CSprite2D.h"
#include "CStaticMesh.h"
#include "CSkinMesh.h"
/******************************/
#include "CSpriteObject.h" 
#include "CUIObject.h"
#include "CStaticMeshObject.h"
#include "CSkinMeshObject.h"
/******************************/
/*********�X�^�e�B�b�N*********/
#include "CGround.h"
/******************************/

/************�X�L��************/
#include "CKaito.h"
#include "CBoss.h"
/******************************/

/**************UI**************/
#include "CKaitoGreenGauge.h"
#include "CKaitoRedGauge.h"
#include "CKaitoBackGauge.h"
#include "CKaitoHpText.h"

#include "CBossHpBackGauge.h"
#include "CBossHpCurrentGauge.h"
#include "CBossHpLateGauge.h"
#include "CBossHpText.h"
#include "CFadeBlack.h"
#include "CStartText.h"
#include "CClickText.h"
/******************************/

/******************************/
#include "CShadowMap.h"		//�e
#include "CConcentLine.h"	//�W����
/******************************/

/******************************/
#include "CSceneManager.h"
#include "CStaticMeshManager.h"
#include "CSprite3DManager.h"
#include "CSprite2DManager.h"
#include "CSkinManager.h"
/******************************/

/********************************************************************************
*	�Q�[���N���X.
**/
class CGame
{
public:
	static constexpr int ENEMY_MAX = 3;	//�G�l�~�[�̍ő吔.

	CGame( CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd );
	~CGame();

	void Create();
	HRESULT LoadData();
	void Release();

	void Update();
	void Draw();

private:
	CDirectX9*		m_pDx9;
	CDirectX11*		m_pDx11;

	//�V�[���}�l�[�W���[�N���X
	CSceneManager*	m_pSceneManager;	

	CDebugText*		m_pDbgText;	//�f�o�b�O�e�L�X�g.

	//�E�B���h�E�n���h��.
	HWND			m_hWnd;

	//�J�������.
	CCamera*		m_pCamera;
	//���C�g
	CLightCamera*	m_pLightCam;

	//�s��.
	D3DXMATRIX		m_mView;	//�r���[(�J����)�s��.
	D3DXMATRIX		m_mProj;	//�ˉe�i�v���W�F�N�V�����j�s��.

	//�n�ʃN���X.
	CGround*			m_pGround;


private:
	//=delete�u�폜��`�v�ƌĂ΂��@�\.
	//�w�肳�ꂽ�ꍇ�A���̊֐��͌Ăяo���Ȃ��Ȃ�.
	CGame() = delete;	//�f�t�H���g�R���X�g���N�^�֎~.
	CGame( const CGame& ) = delete;
	CGame& operator = (const CGame& rhs ) = delete;
};