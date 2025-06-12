#pragma once
#include "CSkinMeshObject.h"

/*******************************************
*		プレイヤークラス
**/

class CKaito
	: public CSkinMeshObject
	, public SingletonManager<CKaito>
{
public:
	const D3DXVECTOR3		StartPos			= D3DXVECTOR3(0.0f, 0.0f, -10.0f);	//ゲームメイン初期位置
	static constexpr double AnimSpeed			= 0.02;		//アニメーションスピード

	static constexpr float	MaxHp				= 100.0f;	//最大体力
	static constexpr float	MoveSlantCont		= 0.71f;	//斜め移動制御値
	static constexpr float	RollingSpeed		= 0.2f;		//回転移動スピード
	static constexpr int	RollingResetTime	= 20;
	static constexpr float	SwordDamage			= 30.0f;	//剣攻撃のダメージ


public:
	//前後移動
	enum enMoveAround
	{
		Move_Around_Stop = 0,	//停止

		Move_Forward,			//前進
		Move_Backward,			//後退
	};
	//左右移動
	enum enMoveAttendDance
	{
		Move_Attend_Stop = 0,	//停止

		Move_Leftward,			//左移動
		Move_Rightward,			//後退
	};

	//プレイヤーのモーション
	enum enPlayerMotion
	{
		P_M_Stop,		//止まる
		P_M_Move,		//移動
		P_M_Rolling,	//回転移動
		P_M_Attack1,	//攻撃一段階
		P_M_Attack2,	//攻撃二段階
		P_M_Attack3,	//攻撃三段階
		P_M_Die,		//倒れる
	};

	//アニメーションナンバー
	enum enPlayerAnim
	{
		P_A_Wait,		//待機
		P_A_Run,		//走る
		P_A_Attack1,	//攻撃１
		P_A_Attack2,	//攻撃２
		P_A_Attack3,	//攻撃３
		P_A_Rolling,	//回転
		P_A_Magic,		//魔法
		P_A_KnockBack,	//ノックバック
		P_A_Dead,		//死
	};


	enum enPlayerAttackColl
	{
		P_C_Attack1 = 0,
		P_C_Attack2,
		P_C_Attack3,
	};


private:
	CKaito();
	~CKaito() override;

public:
	friend class SingletonManager<CKaito>;

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	CCapsuleCollision::Capsule& GetAttackColl(int no) { return m_AttackCapsule[no]; }

	int	GetMotion() { return m_PlayerMotion; }

	bool GetIsDie() { return m_IsDie; }

private:
	//ラジコン操作
	void MoveControl();

	void ChangeMotion(int Motion = 0, int Animation = 0, double ASpeed = AnimSpeed);

	void AttackRange();

private:
	
	bool				m_IsDebug;

	//モーション
	int					m_PlayerMotion;

	//移動
	enMoveAround		m_MoveAround;			//前後移動状態.
	enMoveAttendDance	m_MoveAttendDance;		//左右移動状態.
	float				m_MoveSpeed;		//移動速度.
	float				m_MoveContSpeed;		//斜め移動時制御
	bool				m_IsAround;				
	bool				m_IsAttend;
	float				m_DefaAngle;

	//回転移動
	D3DXVECTOR3			m_RollingvecZ;
	int					m_RollingCoolTime;

	//攻撃当たり判定
	std::vector<CCapsuleCollision::Capsule>		m_AttackCapsule;

	//倒れた判定
	bool				m_IsDie;

};

