#pragma once
#include "CSkinMeshObject.h"
#include "CKaito.h"

/*********************************************
*		�G���N���X
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

	//�ڕW�ʒu���Z�o
	void TargetPosSet(float radius);

	//�^�[�Q�b�g�ւ̕����v�Z(Y���̂�)
	void RotationToTarget(const D3DXVECTOR3& TargetPos, float RotAdjust = 0.0f);

	bool GetIsDie() { return m_IsDie; }


protected:

	int				m_Motion;

	//�ڕW�v���C���[�̈ʒu
	D3DXVECTOR3		m_PlayerLength;
	D3DXVECTOR3		m_TargetPos;
	D3DXVECTOR3		m_TargetLength;

	//�U�����[�V�����ɓ��鎞��
	int				m_AttackMotionTime;

	//�����_���Ń��[�V���������߂�Ƃ��Ɏg��
	int				m_RandomMotion;

	bool			m_IsDie;

	bool		m_IfRandOfGC;


};

