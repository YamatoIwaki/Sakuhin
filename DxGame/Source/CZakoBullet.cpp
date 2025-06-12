#include "CZakoBullet.h"

CZakoBullet::CZakoBullet()
	: m_IsShot		(true)
	, m_AppeaTime	(0)
{}

CZakoBullet::~CZakoBullet()
{
}

void CZakoBullet::Init()
{
}

void CZakoBullet::Update()
{
	if (m_IsShot == true)
	{
			/*****************�x�N�g���v�Z********************/
			//Z���x�N�g��(z+�����ւ̒P�ʃx�N�g��)
			//������(�傫��)�@���P�̃x�N�g����P�ʃx�N�g���Ƃ���.
			D3DXVECTOR3 vecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			//y�����̉�]�s��.
			D3DXMATRIX mRotationY;
			//Y����]�s����쐬.
			D3DXMatrixRotationY(
				&mRotationY,	//(out)�s��.
				m_vRotation.y);	//�v���C���[��Y�����̉�]�l.

			//Y����]�s����g����Z���x�N�g�������W�ω�����.
			D3DXVec3TransformCoord(
				&vecZ,		//(out)Z���x�N�g��.
				&vecZ,		//(in)Z���x�N�g��.
				&mRotationY);	//Y����]�s��.

			/********************************************/

			//�ړ������Z
			m_vPosition -= vecZ * BulletSpeed;

			//�\�����Ԃ����Z
			m_AppeaTime++;


			//�ő�\�����Ԃ𒴂�����~�߂�
			if(m_AppeaTime >= MaxAppeaTime)
			{					
				m_IsShot = false;
			}
	}

	//�����蔻���ݒ�
	m_Capsule.Top = m_vPosition;
	m_Capsule.Bottom = m_vPosition;

}

void CZakoBullet::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	//��΂��Ă��鎞�����\��
	if (m_IsShot == true)
	{
		m_Capsule.HitJudg = true;
		CStaticMeshObject::Draw(View, Proj, Light, Camera);
	}
	else
	{
		m_Capsule.HitJudg = false;
	}
}
