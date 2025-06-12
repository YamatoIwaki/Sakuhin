#pragma once
#include "CEnemy.h"

/***********************************************
*		雑魚クラス.
**/
class CZako
	: public CEnemy
{

public:

	static constexpr float EyePos				= 1.4f;		//地面から目の距離

	static constexpr float RadiusOfTarget		= 10.0f;	//目標からの半径
	static constexpr float OverRadiusOfTarget	= 8.0f;		//停止状態から離れすぎ距離
	static constexpr float MoveSpeed			= 0.004f;	//移動スピード
	static constexpr float MaxAttackCoolTime	= 100.0f;	//攻撃クールタイム

	static constexpr double AnimWaitSpeed		= 0.0002;	//待機中のアニメーションスピード
	static constexpr double AnimMoveSpeed		= 0.00025;	//移動中のアニメーションスピード
	static constexpr double AnimApperSpeed		= 0.0002;	//出現中のアニメーションスピード

	static constexpr double TackleReadySpeed	= 0.005;	//突進攻撃の準備完了合図
	static constexpr double TackleAnimSumSpeed	= 0.00005;	//突進攻撃準備中の加算値
	static constexpr float	TackleSpeed			= 0.02f;	//突進攻撃スピード
	static constexpr float	MaxTackleTime		= 150.0f;	//最大体力

	static constexpr float	MaxHp				= 60.0f;	//最大体力

	static constexpr int	DieRotSpeed			= 10;		//死んだときの回転速度
	static constexpr float	DieSmallSpeed		= 0.00001f;	//死んだときの小さくなる速度
	static constexpr float  DieMinSmallSize		= 0.0001f;	//死んだときの最小の小ささ

public:
	CZako();
	virtual ~CZako() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void DrawShadowMap() override;

	void RayUpdate() override;

	void SetAnimSpeed(double speed) { m_AnimSpeed = speed; }

	bool GetIsShot() { return m_IsShot; }

	void SetIsShot(bool isShot) { m_IsShot = isShot; }
	
	int	GetMotion() { return m_Motion; }

	bool GetIsTackleMove(){ return m_IsTackleMove; }

public:
	enum enZakoMotion
	{
		Z_M_Stop,		//止まる
		Z_M_Move,		//移動
		Z_M_Shot,		//ショットモーション
		Z_M_Tackle,		//体当たり
		Z_M_Die,		//死ぬ
	};

	enum enZakoAnimation
	{
		Z_A_Wait = 0,			//待機
		Z_A_Move,				//移動
		Z_A_Appear,				//出現
	};

private:
	void ChangeMotion(int Motion = 0, int Animation = 0, double ASpeed = AnimSpeed);

	//突進攻撃時の目標位置を算出
	void TacklePosSet();

	//攻撃モーション選択
	void ChoiceRandomMotion();


protected:

	//ショット攻撃
	bool			m_IsShot;


	//突進攻撃
	D3DXVECTOR3		m_TacklePos;	//突進攻撃の目標位置
	D3DXVECTOR3		m_TackleLength;	//突進攻撃の目標位置との長さ
	bool			m_IsTackleMove;	//動いたかどうか
	int				m_TackleTime;	//永遠に目標位置につかない場合

};

