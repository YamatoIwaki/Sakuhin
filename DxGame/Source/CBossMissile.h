#pragma once
#include "CStaticMeshObject.h"


/*******************************************
*		�{�X�̃~�T�C���e
**/


class CBossMissile
	: public CStaticMeshObject
{
public:
	static constexpr int	MaxAppeaTime	= 500;				//�e�̏o������
	static constexpr float	MissileSpeed	= 0.25f;			//�e�̑��x
	static constexpr float	M_RotSpeed		= D3DXToRadian(2);	//��]���x
	static constexpr float	MissileDamage	= 5.0f;				//�e�_���[�W
	static constexpr int	MaxHomingTime	= 80;				//�z�[�~���O���鎞��
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
	//�p�x�̕����֐i��
	void DirectionMove();
	//�ڕW�֕��������X�ɐi��
	void TargetDirection();

	float Clamp(float value, float minVal, float maxVal);

private:
	bool		m_IsShot;

	bool		m_IsHoming;

	int			m_ShotTime;
	int			m_HomingTime;

};

