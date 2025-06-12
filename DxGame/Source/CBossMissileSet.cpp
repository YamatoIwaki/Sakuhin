#include "CBossMissileSet.h"

CBossMissileSet::CBossMissileSet()
	: m_BossMissile			()
{

	//最初に４つのミサイル弾を生成
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
				//ミサイル弾の当たり判定
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

	//状態をセットする
	for (int i = 0; i < m_BossMissile.size(); i++)
	{
		m_BossMissile[i]->ResetShotTime();

		m_BossMissile[i]->SetIsShotting(true);
		m_BossMissile[i]->SetIsHoming(true);
	}

	float rY = CBoss::GetInstance()->GetRotation().y;

	//最初の角度をそれぞれセットする
	m_BossMissile[0]->SetRotation(0.0f, rY + D3DXToRadian(-30), 0.0f);
	m_BossMissile[1]->SetRotation(0.0f, rY + D3DXToRadian(-30), 0.0f);
	m_BossMissile[2]->SetRotation(0.0f, rY + D3DXToRadian(30), 0.0f);
	m_BossMissile[3]->SetRotation(0.0f, rY + D3DXToRadian(30), 0.0f);

	//最初の位置をそれぞれセットする
	//ベクトル方向
	D3DXVECTOR3 vecAxisX(1.0f, 0.0f, 0.0f);

	//y方向の回転行列.
	D3DXMATRIX mRotationY;
	//Y軸回転行列を作成.
	D3DXMatrixRotationY(
		&mRotationY,	//(out)行列.
		rY);	//プレイヤーのY方向の回転値.

	//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
	D3DXVec3TransformCoord(
		&vecAxisX,		//(out)Z軸ベクトル.
		&vecAxisX,		//(in)Z軸ベクトル.
		&mRotationY);	//Y軸回転行列.


	m_BossMissile[0]->SetPosition(CBoss::GetInstance()->GetPosition() + vecAxisX * 1.0f);
	m_BossMissile[1]->SetPosition(CBoss::GetInstance()->GetPosition() + vecAxisX * 0.5f);
	m_BossMissile[2]->SetPosition(CBoss::GetInstance()->GetPosition() + vecAxisX * -0.5f);
	m_BossMissile[3]->SetPosition(CBoss::GetInstance()->GetPosition() + vecAxisX * -1.0f);

	m_BossMissile[0]->SetPosition(m_BossMissile[0]->GetPosition().x, CBoss::EyePos, m_BossMissile[0]->GetPosition().z);
	m_BossMissile[1]->SetPosition(m_BossMissile[1]->GetPosition().x, CBoss::EyePos, m_BossMissile[1]->GetPosition().z);
	m_BossMissile[2]->SetPosition(m_BossMissile[2]->GetPosition().x, CBoss::EyePos, m_BossMissile[2]->GetPosition().z);
	m_BossMissile[3]->SetPosition(m_BossMissile[3]->GetPosition().x, CBoss::EyePos, m_BossMissile[3]->GetPosition().z);

}
