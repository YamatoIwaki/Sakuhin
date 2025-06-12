#include "CBossMissileSet.h"

CBossMissileSet::CBossMissileSet()
	: m_BossMissile			()
{

	//�ŏ��ɂS�̃~�T�C���e�𐶐�
	for (int i = 0; i < MissileMax; i++)
	{
		m_BossMissile.push_back(new CBossMissile());

		m_BossMissile[i]->AttachMesh(*CStaticMeshManager::GetInstance()->GetMesh(CStaticMeshManager::enList::Missile));
	}
}

CBossMissileSet::~CBossMissileSet()
{
}

void CBossMissileSet::Update()
{
	if (CBoss::GetInstance()->GetMissile() == true)
	{
		CSoundManager::PlaySE(CSoundManager::SE_B_Missile);

		SetMissile();
	}

	for (size_t m = 0; m < m_BossMissile.size(); m++)
	{
		if (m_BossMissile[m]->GetIsShotting() == true)
		{
			if (CCapsuleCollision::IsHit(CKaito::GetInstance()->GetCapsule(), m_BossMissile[m]->GetCapsule()))
			{
				//�~�T�C���e�̓����蔻��
				m_BossMissile[m]->SetIsShotting(false);
				if (CKaito::GetInstance()->GetMotion() != CKaito::P_M_Die)
				{
					CSoundManager::PlaySE(CSoundManager::SE_P_Damage);

					CKaito::GetInstance()->Damage(CBossMissile::MissileDamage);
				}
			}
		}
	}



	for (auto& m : m_BossMissile)
	{
		m->Update();
	}


}

void CBossMissileSet::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	for (auto& m : m_BossMissile)
	{
		m->Draw(View, Proj, Light, Camera);
	}
}

void CBossMissileSet::SetMissile()
{
	CBoss::GetInstance()->SetIsMissile(false);

	//��Ԃ��Z�b�g����
	for (int i = 0; i < m_BossMissile.size(); i++)
	{
		m_BossMissile[i]->ResetShotTime();

		m_BossMissile[i]->SetIsShotting(true);
		m_BossMissile[i]->SetIsHoming(true);
	}

	float rY = CBoss::GetInstance()->GetRotation().y;

	//�ŏ��̊p�x�����ꂼ��Z�b�g����
	m_BossMissile[0]->SetRotation(0.0f, rY + D3DXToRadian(-30), 0.0f);
	m_BossMissile[1]->SetRotation(0.0f, rY + D3DXToRadian(-30), 0.0f);
	m_BossMissile[2]->SetRotation(0.0f, rY + D3DXToRadian(30), 0.0f);
	m_BossMissile[3]->SetRotation(0.0f, rY + D3DXToRadian(30), 0.0f);

	//�ŏ��̈ʒu�����ꂼ��Z�b�g����
	//�x�N�g������
	D3DXVECTOR3 vecAxisX(1.0f, 0.0f, 0.0f);

	//y�����̉�]�s��.
	D3DXMATRIX mRotationY;
	//Y����]�s����쐬.
	D3DXMatrixRotationY(
		&mRotationY,	//(out)�s��.
		rY);	//�v���C���[��Y�����̉�]�l.

	//Y����]�s����g����Z���x�N�g�������W�ω�����.
	D3DXVec3TransformCoord(
		&vecAxisX,		//(out)Z���x�N�g��.
		&vecAxisX,		//(in)Z���x�N�g��.
		&mRotationY);	//Y����]�s��.


	m_BossMissile[0]->SetPosition(CBoss::GetInstance()->GetPosition() + vecAxisX * 1.0f);
	m_BossMissile[1]->SetPosition(CBoss::GetInstance()->GetPosition() + vecAxisX * 0.5f);
	m_BossMissile[2]->SetPosition(CBoss::GetInstance()->GetPosition() + vecAxisX * -0.5f);
	m_BossMissile[3]->SetPosition(CBoss::GetInstance()->GetPosition() + vecAxisX * -1.0f);

	m_BossMissile[0]->SetPosition(m_BossMissile[0]->GetPosition().x, CBoss::EyePos, m_BossMissile[0]->GetPosition().z);
	m_BossMissile[1]->SetPosition(m_BossMissile[1]->GetPosition().x, CBoss::EyePos, m_BossMissile[1]->GetPosition().z);
	m_BossMissile[2]->SetPosition(m_BossMissile[2]->GetPosition().x, CBoss::EyePos, m_BossMissile[2]->GetPosition().z);
	m_BossMissile[3]->SetPosition(m_BossMissile[3]->GetPosition().x, CBoss::EyePos, m_BossMissile[3]->GetPosition().z);

}
