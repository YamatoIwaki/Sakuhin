#pragma once
#include "CEnemy.h"

/*********************************************
*				ボスクラス
**/



class CBoss
	: public CEnemy
	, public SingletonManager<CBoss>
{
public:


	static constexpr float	MaxHp					= 800.0f;	//最大体力
	static constexpr float	MoveSpeed				= 0.01f;	//移動スピード
	static constexpr float  SordAttackMoveSpeed		= 0.04f;	//剣攻撃時の移動スピード
	static constexpr int	StepCoolTime			= 150;		//サイド、後ろにステップする時間
	static constexpr float	StepSpeed				= 0.1f;		//スライドスピード
	static constexpr int	MaxStepTime				= 65;		//マックススライド時間

	static constexpr double AnimSpeed				= 0.006;	//アニメーションスピード

	static constexpr int	AttackCoolTime			= 180;		//攻撃する時間
	static constexpr float	MaxAttackLength			= 8.0f;		//攻撃モーションの距離の最大の長さ

	static constexpr float	RadiusOfSwordAttack		= 1.0f;		//目標からの半径(剣攻撃の場合)
	static constexpr float	RadiusOfTargetDefalt	= 8.0f;		//目標からの半径

	static constexpr float  EyePos					= 3.8f;		//目の位値

private:
	CBoss();
	~CBoss() override;
public:
	friend class SingletonManager<CBoss>;
	
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void DrawShadowMap() override;

	void RayUpdate() override;

	bool GetMissile() { return m_IsMissile; }

	void SetIsMissile(bool isMissile) { m_IsMissile = isMissile; }

	bool GetIsSAttack() { return m_IsSAttack; }

	void SetIsSAttack(bool isSAttack) { m_IsSAttack = isSAttack; }

public:

	enum enBossMotion
	{
		B_M_Boot,			//起動
		B_M_Wait,			//待機
		B_M_Straight,		//前移動
		B_M_RightStep,		//右ステップ
		B_M_LeftStep,		//左ステップ
		B_M_BackStep,		//バックステップ
		B_M_SwordAttack,	//近接攻撃
		B_M_Missile,		//ミサイル攻撃
		B_M_Die,			//死んでいる


	};

	enum enBossAnimation
	{
		B_A_Boot,			//起動
		B_A_Wait,			//待機
		B_A_Wince,			//怯み
		B_A_SwordAttack,	//近接攻撃
		B_A_Missile,		//ミサイル攻撃
		B_A_BeginGuard,		//ガード出しはじめ
		B_A_Guard,			//ガード中
		B_A_BeginStraight,	//前に進みだす
		B_A_Straight,		//前に進んでいる
		B_A_EndStraight,	//止まる時
		B_A_BeginRightStep,	//右に進みだす
		B_A_RightStep,		//右に進んでいる
		B_A_EndRightStep,	//止まる時
		B_A_BeginLeftStep,	//左に進みだす
		B_A_LeftStep,		//左に進んでいる
		B_A_EndLeftStep,	//止まる時
		B_A_BeginBackStep,	//後ろ下がる
		B_A_BackStep,		//後ろに下がっている
		B_A_EndBackStep,	//止まる時
		B_A_BeginLose,		//負け動作
		B_A_Lose,			//負けポーズ
		B_A_BeginWin,		//勝ち動作
		B_A_Win,			//勝ちポーズ
	};

private:
	void ChangeMotion(int Motion = 1, int Animation = 1, double ASpeed = AnimSpeed);

	//ステップの方向を決める
	void WhatStep();

	//攻撃モーションを決める
	void WhatAttack();

	std::vector<double> CulculateWeightsFromDistance(float distance);

protected:

	bool		m_IsApper;		//出現しているかどうか

	int			m_StepCoolTime;	//スライドクールタイム時間
	int			m_StepTime;		//スライド中時間

	bool		m_IsMissile;	//発射したかどうか
	bool		m_IsSetMissile;	//セット中かどうか

	bool		m_IsSAttack;	//剣攻撃の当たり判定があるかどうか
	bool		m_ChangeSAttack;//当たり判定が一度切り替わったかどうか

};

