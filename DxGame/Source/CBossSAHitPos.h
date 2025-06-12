#pragma once
#include "CStaticMeshObject.h"

/*******************************************
*	�{�X�̌��U���̓����蔻��
**/


class CBossSAHitPos
	: public CStaticMeshObject
	, public SingletonManager<CBossSAHitPos>
{
public:
	static constexpr float SADamage = 15.0f; //�^����_���[�W

private:
	CBossSAHitPos();
	virtual ~CBossSAHitPos() override;
public:
	friend SingletonManager <CBossSAHitPos>;


	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	bool GetIsHitAttack() { return m_IsHitAttack; }

	void SetIsHitAttack(bool IsHit) { m_IsHitAttack = IsHit; }

	void SetTargetPos(D3DXVECTOR3 Pos) { m_TargetPos = Pos; }

	void SetTargetRot(float Rot) { m_TargetRotY = Rot; }

private:

	bool			m_IsHitAttack;		//�����������̔���

	D3DXVECTOR3		m_TargetPos;		//�Ώۂ̉�]�l	

	float			m_TargetRotY;		//�Ώۂ̉�]�l	

};

