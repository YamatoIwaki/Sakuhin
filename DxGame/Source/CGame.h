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
/*********スタティック*********/
#include "CGround.h"
/******************************/

/************スキン************/
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
#include "CShadowMap.h"		//影
#include "CConcentLine.h"	//集中線
/******************************/

/******************************/
#include "CSceneManager.h"
#include "CStaticMeshManager.h"
#include "CSprite3DManager.h"
#include "CSprite2DManager.h"
#include "CSkinManager.h"
/******************************/

/********************************************************************************
*	ゲームクラス.
**/
class CGame
{
public:
	static constexpr int ENEMY_MAX = 3;	//エネミーの最大数.

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

	//シーンマネージャークラス
	CSceneManager*	m_pSceneManager;	

	CDebugText*		m_pDbgText;	//デバッグテキスト.

	//ウィンドウハンドル.
	HWND			m_hWnd;

	//カメラ情報.
	CCamera*		m_pCamera;
	//ライト
	CLightCamera*	m_pLightCam;

	//行列.
	D3DXMATRIX		m_mView;	//ビュー(カメラ)行列.
	D3DXMATRIX		m_mProj;	//射影（プロジェクション）行列.

	//地面クラス.
	CGround*			m_pGround;


private:
	//=delete「削除定義」と呼ばれる機能.
	//指定された場合、その関数は呼び出せなくなる.
	CGame() = delete;	//デフォルトコンストラクタ禁止.
	CGame( const CGame& ) = delete;
	CGame& operator = (const CGame& rhs ) = delete;
};