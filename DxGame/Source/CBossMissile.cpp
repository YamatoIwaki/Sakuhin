#include "CBossMissile.h"
#include "CKaito.h"

CBossMissile::CBossMissile()
	: m_IsShot		(false)
	, m_IsHoming	(false)
	, m_ShotTime	(0)
	, m_HomingTime	()
{
}

CBossMissile::~CBossMissile()
{
}

void CBossMissile::Init()
{
}

void CBossMissile::Update()
{

	if (m_IsShot == true)
	{
		//�\�����Ԃ����Z
		m_ShotTime++;


		//�ő�\�����Ԃ𒴂�����~�߂�
		if (m_ShotTime >= MaxAppeaTime)
		{
			m_ShotTime = 0;
			m_IsShot = false;
		}

		TargetDirection();

		DirectionMove();

	}
	

	//�����蔻���ݒ�
	m_Capsule.Top = m_vPosition;
	m_Capsule.Bottom = m_vPosition;

}

void CBossMissile::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
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

void CBossMissile::DirectionMove()
{
	/*****************�x�N�g���v�Z********************/
//Z���x�N�g��(z+�����ւ̒P�ʃx�N�g��)
//������(�傫��)�@���P�̃x�N�g����P�ʃx�N�g���Ƃ���.
	D3DXVECTOR3 vecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//y�����̉�]�s��.
	D3DXMATRIX mRotationX, mRotationY, mRotationZ;
	//Y����]�s����쐬.
	D3DXMatrixRotationX(&mRotationX, m_vRotation.x);
	D3DXMatrixRotationY(&mRotationY, m_vRotation.y);
	D3DXMatrixRotationZ(&mRotationZ, m_vRotation.z);

	D3DXMATRIX mRot = mRotationY * mRotationX * mRotationZ;

	//Y����]�s����g����Z���x�N�g�������W�ω�����.
	D3DXVec3TransformCoord(
		&vecZ,		//(out)Z���x�N�g��.
		&vecZ,		//(in)Z���x�N�g��.
		&mRot);	//Y����]�s��.

	/********************************************/

	//�ړ������Z
	m_vPosition -= vecZ * MissileSpeed;


}

void CBossMissile::TargetDirection()
{
	D3DXVECTOR3 toTarget = CKaito::GetInstance()->GetPosition() - m_vPosition;
	D3DXVec3Normalize(&toTarget, &toTarget);

	if (m_IsHoming == true && m_HomingTime > MaxHomingTime)
	{
		m_HomingTime = 0;
		m_IsHoming = false;
	}

	//�����̍X�V
	if(m_IsHoming == true)
	{
		m_HomingTime++;

		D3DXVECTOR3 direction = CKaito::GetInstance()->GetPosition() - m_vPosition;
		
		//�^�[�Q�b�g�̐^�񒆂Ɍ������Ăق����̂ŏ�����ɂ���
		direction.y += 0.5f;

		//Y���v�Z
		D3DXVECTOR3 dirFlat = direction;
		dirFlat.y = 0;

		//�������[���̏ꍇ�͉������Ȃ�
		if (D3DXVec3Length(&dirFlat) > 0.0004f)
		{

			//�p�x�v�Z
			float targetAngle = atan2f(direction.x, direction.z);

			//�p�x�������߂�
			float angleDiffY = targetAngle - m_vRotation.y;

			while (angleDiffY > D3DX_PI)	angleDiffY -= D3DX_PI * 2.0f;
			while (angleDiffY < -D3DX_PI)	angleDiffY += D3DX_PI * 2.0f;

			//��]�ʂ��v�Z
			float rotAmountY = angleDiffY * min(M_RotSpeed, 1.0f);

			//�p�x���X�V
			m_vRotation.y -= rotAmountY;
		}

		//X���v�Z
		D3DXVec3Normalize(&direction, &direction);

		//Y����]�������t��]���ă��[�J����Ԃɕϊ�
		D3DXMATRIX rotYInv;
		D3DXMatrixRotationY(&rotYInv, -m_vRotation.y);

		D3DXVECTOR3 localDir;
		D3DXVec3TransformNormal(&localDir, &direction, &rotYInv);

		float targetXAngle;

		//Y�����}90�ȏ�̎����]��h�����߂̏���
		if (fabsf(m_vRotation.y) >= D3DX_PI / 2.0f)
		{
			targetXAngle = -atan2f(fabsf(localDir.y), localDir.z);
		}
		else
		{
			targetXAngle = -atan2f(localDir.y, localDir.z);
		}


		float angleDiffX = targetXAngle - m_vRotation.x;

		while (angleDiffX > D3DX_PI)	angleDiffX -= D3DX_PI * 2.0f;
		while (angleDiffX < -D3DX_PI)	angleDiffX += D3DX_PI * 2.0f;

		//��]�ʂ��v�Z
		float rotAmountX = angleDiffX * min(M_RotSpeed, 1.0f);

		//�p�x���X�V
		m_vRotation.x -= rotAmountX;

	}


}

float CBossMissile::Clamp(float value, float minVal, float maxVal)
{
	return (std::max)(minVal, (std::min)(maxVal, value));
}
