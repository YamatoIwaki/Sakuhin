#pragma once
#include "CEnemy.h"

/***************************************************
*			ザコ２クラス
**/


class CZako2
	: public CEnemy
{
public:

	static constexpr float			MaxHp				= 100.0f;	//最大Hp
	static constexpr double			AnimSpeed			= 0.003;
	static constexpr float			MoveSpeed			= 0.01f;	//移動スピード
	static constexpr float			SrideSpeed			= 0.1f;		//スライドスピード
	static constexpr int			MaxSrideTime		= 65;		//マックススライド時間
	static constexpr float			MaxAttackCoolTime	= 100.0f;	//攻撃クールタイム

	static constexpr float			RadiusOfTarget		= 0.8f;		//目標からの半径
	static constexpr float			OverRadiusOfTarget	= 2.0f;		//目標から離れすぎ判定
	
	static constexpr double			IsAttackHitBegin	= 0.2;		//攻撃判定始まり
	static constexpr double			IsAttackHitEnd		= 0.5;		//攻撃判定終わり

	static constexpr int			DieRotSpeed			= 10;		//死んだときの回転速度
	static constexpr float			DieSmallSpeed		= 0.01f;	//死んだときの小さくなる速度
	static constexpr float			DieMinSmallSize		= 0.0001f;	//死んだときの最小の小ささ



public:
	CZako2();
	virtual ~CZako2() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void RayUpdate() override;

	void DrawShadowMap() override;

	bool GetIsAttack() { return m_IsAttack; }

	void SetIsAttack(bool isAttack) { m_IsAttack = isAttack; }


public:
	enum enZako2Motion
	{
		Z2_M_Stop,		//止まる
		Z2_M_Move,		//移動
		Z2_M_Attack,	//攻撃
		Z2_M_RightSride,//右スライド
		Z2_M_LeftSride,	//左スライド
		Z2_M_Die,		//死んでいる
	};

	enum enZako2Animation
	{
		Z2_A_Wait = 0,			//待機
		Z2_A_Move,				//移動
		Z2_A_StreetSride,		//前進スライド
		Z2_A_ArmAttack,			//両腕攻撃
		Z2_A_RightSride,			//左スライド
		Z2_A_LeftSride,		//右スライド
		Z2_A_Jump,				//ジャンプ
		Z2_A_Wait2,				//待機２
		Z2_A_FallDown,			//倒れる
		Z2_A_GetUp,				//起き上がる
		Z2_A_OneArmAttack,		//片腕攻撃
	};

private:

	void ChangeMotion(int Motion = 0, int Animation = 0, double ASpeed = AnimSpeed);

	//モーション選択
	void ChoiceRandomMotion();

private:

	bool		m_IsAttack;		//攻撃しているかどうか

	int			m_SrideTime;	//スライド時間


};

