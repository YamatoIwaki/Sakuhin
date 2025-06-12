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

		//y方向の回転行列.
		D3DXMATRIX mRotationY;
		//Y軸回転行列を作成.
		D3DXMatrixRotationY(
			&mRotationY,	//(out)行列.
			m_TargetRotY);	//プレイヤーのY方向の回転値.

		//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
		D3DXVec3TransformCoord(
			&vecAxisZ,		//(out)Z軸ベクトル.
			&vecAxisZ,		//(in)Z軸ベクトル.
			&mRotationY);	//Y軸回転行列.

		//当たり判定の調整
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
