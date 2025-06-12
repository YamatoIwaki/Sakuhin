#pragma once
#include "CStaticMeshObject.h"


/*******************************************
*		ボスのミサイル弾
**/


class CBossMissile
	: public CStaticMeshObject
{
public:
	static constexpr int	MaxAppeaTime	= 500;				//弾の出現時間
	static constexpr float	MissileSpeed	= 0.25f;			//弾の速度
	static constexpr float	M_RotSpeed		= D3DXToRadian(2);	//回転速度
	static constexpr float	MissileDamage	= 5.0f;				//弾ダメージ
	static constexpr int	MaxHomingTime	= 80;				//ホーミングする時間
public:
	CBossMissile();
	virtual ~CBossMissile() override;

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;


	void SetIsShotting(bool s) { m_IsShot = s; }
	bool GetIsShotting() { return m_IsShot; }
	void SetIsHoming(bool h) { m_IsHoming = h; }

	void ResetShotTime(){
		m_ShotTime = 0;
	}

private:
	//角度の方向へ進む
	void DirectionMove();
	//目標へ方向を徐々に進む
	void TargetDirection();

	float Clamp(float value, float minVal, float maxVal);

private:
	bool		m_IsShot;

	bool		m_IsHoming;

	int			m_ShotTime;
	int			m_HomingTime;

};

