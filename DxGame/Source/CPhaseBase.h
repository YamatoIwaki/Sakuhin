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
#include "CZakoBullet.h"
#include "CZako2AttackHitPoint.h"
#include "CBossSAHitPos.h"
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
#include "CBossHpBackGauge.h"
#include "CBossHpCurrentGauge.h"
#include "CBossHpLateGauge.h"
#include "CBossHpText.h"
#include "CPhaseTextBase.h"
#include "CStartText.h"
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

/*************************************
*		フェーズ基礎クラス
**/


class CPhaseBase
{
public:
	CPhaseBase();
	virtual ~CPhaseBase();

	virtual void Init() = 0;

	virtual void Update() = 0;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) = 0;

	virtual void DrawShadow() = 0;


	virtual void Exit() = 0;

	bool GetEndPhase() { return m_IsEndPhase; }

protected:

	void CreateZako(float x, float y, float z);

	void CreateZako2(float x, float y, float z);

	void CreateZakoBullet(int no);

	void ZakoSetUpdate();

	void Zako2SetUpdate();

	void ZakoSetDraw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera);

	void Zako2SetDraw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera);

	bool LoadEnemyPos(const std::string& filename);

protected:
	//スプライト3Dオブジェクト
	std::vector<CZakoRedGauge*>		m_ZakoRedGauge;
	std::vector<CZakoBackGauge*>	m_ZakoBackGauge;

	std::vector<CZako2RedGauge*>	m_Zako2RedGauge;
	std::vector<CZako2BackGauge*>	m_Zako2BackGauge;

	//スプライト２D
	std::unique_ptr<CPhaseTextBase>	m_pPhaseText;


	//スキンメッシュオブジェクト
	std::vector<CZako*>				m_Zako;
	std::vector<CZako2*>			m_Zako2;

	//すべての敵が死んだかどうか判定
	std::vector<bool>				m_IsZakoDie;

	std::vector<bool>				m_IsZako2Die;

	std::vector<CZakoBullet*>				m_ZakoBullet;

	std::vector<CZako2AttackHitPoint*>		m_Zako2_A_HitPoint;

	bool							m_IsEndPhase;


};

