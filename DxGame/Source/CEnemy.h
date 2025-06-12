#pragma once
#include "CSkinMeshObject.h"
#include "CKaito.h"

/*********************************************
*		敵基底クラス
**/


class CEnemy
	: public CSkinMeshObject
{
public:
	static constexpr double	AnimSpeed = 0.0002;


public:
	CEnemy();
	virtual ~CEnemy() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	virtual void DrawShadowMap();

	virtual void QuaterDraw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera);

	virtual void RayUpdate();

	//目標位置を算出
	void TargetPosSet(float radius);

	//ターゲットへの方向計算(Y軸のみ)
	void RotationToTarget(const D3DXVECTOR3& TargetPos, float RotAdjust = 0.0f);

	bool GetIsDie() { return m_IsDie; }


protected:

	int				m_Motion;

	//目標プレイヤーの位置
	D3DXVECTOR3		m_PlayerLength;
	D3DXVECTOR3		m_TargetPos;
	D3DXVECTOR3		m_TargetLength;

	//攻撃モーションに入る時間
	int				m_AttackMotionTime;

	//ランダムでモーションを決めるときに使う
	int				m_RandomMotion;

	bool			m_IsDie;

	bool		m_IfRandOfGC;


};

