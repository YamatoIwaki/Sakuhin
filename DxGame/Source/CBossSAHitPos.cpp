#include "CBossSAHitPos.h"
#include "CBoss.h"
#include "CKaito.h"

CBossSAHitPos::CBossSAHitPos()
{
	m_vScale *= 5.0f;
}

CBossSAHitPos::~CBossSAHitPos()
{
}

void CBossSAHitPos::Init()
{
}

void CBossSAHitPos::Update()
{
	if (CBoss::GetInstance()->GetIsSAttack() == true)
	{
		D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);

		//y�����̉�]�s��.
		D3DXMATRIX mRotationY;
		//Y����]�s����쐬.
		D3DXMatrixRotationY(
			&mRotationY,	//(out)�s��.
			CBoss::GetInstance()->GetRotation().y);	//�v���C���[��Y�����̉�]�l.

		//Y����]�s����g����Z���x�N�g�������W�ω�����.
		D3DXVec3TransformCoord(
			&vecAxisZ,		//(out)Z���x�N�g��.
			&vecAxisZ,		//(in)Z���x�N�g��.
			&mRotationY);	//Y����]�s��.

		//�����蔻��̒���
		m_vPosition = CBoss::GetInstance()->GetPosition() - (vecAxisZ * 2.0f);
		m_vPosition.y += 1.0f;

		m_Capsule.HitJudg = true;

		//�����蔻��
		if (CCapsuleCollision::IsHit(m_Capsule, CKaito::GetInstance()->GetCapsule()))
		{
			CSoundManager::PlaySE(CSoundManager::SE_P_Damage);

			CBoss::GetInstance()->SetIsSAttack(false);
			CKaito::GetInstance()->Damage(SADamage);
		}
	}
	else
	{
		m_Capsule.HitJudg = false;
	}
		
	m_Capsule.Radius	= 1.0f;
	m_Capsule.Top		= m_vPosition;
	m_Capsule.Bottom	= m_vPosition;

}

void CBossSAHitPos::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}
