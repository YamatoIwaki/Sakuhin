#pragma once
#include "CDirectX11.h"
#include "CDirectX9.h"


//--------------------------------------//
#include "CCamera.h"
//--------------------------------------//

//--------------------------------------//
#include "CDebugText.h"
//--------------------------------------//

//-------------SpriteObject-------------//
#include "CSprite3D.h"
#include "CZakoRedGauge.h"
#include "CZakoBackGauge.h"
#include "CZako2RedGauge.h"
#include "CZako2BackGauge.h"
//--------------------------------------//

//--------------StaticMesh--------------//
#include "CStaticMesh.h"
#include "CGround.h"
#include "CApperGround.h"
#include "CZakoBullet.h"
#include "CZako2AttackHitPoint.h"
#include "CBossSAHitPos.h"
#include "CApperGround.h"
//--------------------------------------//

//---------------SkinMesh---------------//
#include "CSkinMesh.h"
#include "CZako.h"
#include "CKaito.h"
#include "CZako2.h"
#include "CBoss.h"
//--------------------------------------//

//------------------UI------------------//
#include "CSprite2D.h"
#include "CKaitoGreenGauge.h"
#include "CKaitoRedGauge.h"
#include "CKaitoBackGauge.h"
#include "CKaitoHpText.h"

#include "CBossHpBackGauge.h"
#include "CBossHpCurrentGauge.h"
#include "CBossHpLateGauge.h"
#include "CBossHpText.h"

#include "CFadeBlack.h"
#include "CClickText.h"
//--------------------------------------//


//--------------当たり判定--------------//
#include"CCapsuleCollision.h"
//--------------------------------------//

//--------------------------------------//
#include "CShadowMap.h"					//影
#include "CConcentLine.h"				//集中線
//--------------------------------------//

//--------------マネージャー------------//
#include "CSoundManager.h"				//音
#include "CStaticMeshManager.h"			//スタティックメッシュ
#include "CSprite3DManager.h"			//スプライト3D
#include "CSprite2DManager.h"			//スプライト2D
#include "CSkinManager.h"				//スキンメッシュ
//--------------------------------------//

//------------ミサイルの動き------------//
#include "CBossMissileSet.h"
//--------------------------------------//



/*******************************
*		シーン基底クラス
**/

class CSceneBase
{
public:
	CSceneBase();
	virtual ~CSceneBase();


	virtual void Init() = 0;

	virtual void Update() = 0;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) = 0;

	virtual void Exit() = 0;

protected:
	void CreateZako(float x, float y, float z, enScene scene = enScene::Title);

	void CreateZako2(float x, float y, float z, enScene scene = enScene::Title);


protected:

	CDirectX11*		m_pDx11;
	CDirectX9*		m_pDx9;

	//スタティックメッシュオブジェクト
	CGround*		m_pGround;
	std::unique_ptr<CApperGround>	m_pStage;
	std::unique_ptr<CApperGround>	m_pFence;

	//スキンメッシュオブジェクト
	std::vector<CZako*>				m_Zako;
	std::vector<CZako2*>			m_Zako2;
	CKaito*							m_pKaito;
	CBoss*							m_pBoss;

};

