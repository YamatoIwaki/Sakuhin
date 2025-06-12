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

		//y方向の回転行列.
		D3DXMATRIX mRotationY;
		//Y軸回転行列を作成.
		D3DXMatrixRotationY(
			&mRotationY,	//(out)行列.
			CBoss::GetInstance()->GetRotation().y);	//プレイヤーのY方向の回転値.

		//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
		D3DXVec3TransformCoord(
			&vecAxisZ,		//(out)Z軸ベクトル.
			&vecAxisZ,		//(in)Z軸ベクトル.
			&mRotationY);	//Y軸回転行列.

		//当たり判定の調整
		m_vPosition = CBoss::GetInstance()->GetPosition() - (vecAxisZ * 2.0f);
		m_vPosition.y += 1.0f;

		m_Capsule.HitJudg = true;

		//当たり判定
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
