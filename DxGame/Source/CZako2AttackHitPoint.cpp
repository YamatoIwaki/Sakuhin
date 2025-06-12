#include "CZako2AttackHitPoint.h"

CZako2AttackHitPoint::CZako2AttackHitPoint()
	: m_IsHitAttack	(false)
	, m_TargetRotY	()
{
	m_vScale *= 3.0f;

}

CZako2AttackHitPoint::~CZako2AttackHitPoint()
{
}

void CZako2AttackHitPoint::Init()
{
}

void CZako2AttackHitPoint::Update()
{
	if (m_IsHitAttack == true)
	{
		D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);

		//y�����̉�]�s��.
		D3DXMATRIX mRotationY;
		//Y����]�s����쐬.
		D3DXMatrixRotationY(
			&mRotationY,	//(out)�s��.
			m_TargetRotY);	//�v���C���[��Y�����̉�]�l.

		//Y����]�s����g����Z���x�N�g�������W�ω�����.
		D3DXVec3TransformCoord(
			&vecAxisZ,		//(out)Z���x�N�g��.
			&vecAxisZ,		//(in)Z���x�N�g��.
			&mRotationY);	//Y����]�s��.

		//�����蔻��̒���
		m_vPosition = m_TargetPos + (vecAxisZ * 2.0f);
		m_vPosition.y += 1.0f;
	}

	m_Capsule.Top	 = m_vPosition;
	m_Capsule.Bottom = m_vPosition;

}

void CZako2AttackHitPoint::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);

}
