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
			/*****************ベクトル計算********************/
			//Z軸ベクトル(z+方向への単位ベクトル)
			//※長さ(大きさ)　が１のベクトルを単位ベクトルという.
			D3DXVECTOR3 vecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			//y方向の回転行列.
			D3DXMATRIX mRotationY;
			//Y軸回転行列を作成.
			D3DXMatrixRotationY(
				&mRotationY,	//(out)行列.
				m_vRotation.y);	//プレイヤーのY方向の回転値.

			//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
			D3DXVec3TransformCoord(
				&vecZ,		//(out)Z軸ベクトル.
				&vecZ,		//(in)Z軸ベクトル.
				&mRotationY);	//Y軸回転行列.

			/********************************************/

			//移動を加算
			m_vPosition -= vecZ * BulletSpeed;

			//表示時間を加算
			m_AppeaTime++;


			//最大表示時間を超えたら止める
			if(m_AppeaTime >= MaxAppeaTime)
			{					
				m_IsShot = false;
			}
	}

	//当たり判定を設定
	m_Capsule.Top = m_vPosition;
	m_Capsule.Bottom = m_vPosition;

}

void CZakoBullet::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	//飛ばしている時だけ表示
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
