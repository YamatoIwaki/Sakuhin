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
		//表示時間を加算
		m_ShotTime++;


		//最大表示時間を超えたら止める
		if (m_ShotTime >= MaxAppeaTime)
		{
			m_ShotTime = 0;
			m_IsShot = false;
		}

		TargetDirection();

		DirectionMove();

	}
	

	//当たり判定を設定
	m_Capsule.Top = m_vPosition;
	m_Capsule.Bottom = m_vPosition;

}

void CBossMissile::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
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

void CBossMissile::DirectionMove()
{
	/*****************ベクトル計算********************/
//Z軸ベクトル(z+方向への単位ベクトル)
//※長さ(大きさ)　が１のベクトルを単位ベクトルという.
	D3DXVECTOR3 vecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//y方向の回転行列.
	D3DXMATRIX mRotationX, mRotationY, mRotationZ;
	//Y軸回転行列を作成.
	D3DXMatrixRotationX(&mRotationX, m_vRotation.x);
	D3DXMatrixRotationY(&mRotationY, m_vRotation.y);
	D3DXMatrixRotationZ(&mRotationZ, m_vRotation.z);

	D3DXMATRIX mRot = mRotationY * mRotationX * mRotationZ;

	//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
	D3DXVec3TransformCoord(
		&vecZ,		//(out)Z軸ベクトル.
		&vecZ,		//(in)Z軸ベクトル.
		&mRot);	//Y軸回転行列.

	/********************************************/

	//移動を加算
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

	//向きの更新
	if(m_IsHoming == true)
	{
		m_HomingTime++;

		D3DXVECTOR3 direction = CKaito::GetInstance()->GetPosition() - m_vPosition;
		
		//ターゲットの真ん中に向かってほしいので少し上にする
		direction.y += 0.5f;

		//Y軸計算
		D3DXVECTOR3 dirFlat = direction;
		dirFlat.y = 0;

		//方向がゼロの場合は何もしない
		if (D3DXVec3Length(&dirFlat) > 0.0004f)
		{

			//角度計算
			float targetAngle = atan2f(direction.x, direction.z);

			//角度差を求める
			float angleDiffY = targetAngle - m_vRotation.y;

			while (angleDiffY > D3DX_PI)	angleDiffY -= D3DX_PI * 2.0f;
			while (angleDiffY < -D3DX_PI)	angleDiffY += D3DX_PI * 2.0f;

			//回転量を計算
			float rotAmountY = angleDiffY * min(M_RotSpeed, 1.0f);

			//角度を更新
			m_vRotation.y -= rotAmountY;
		}

		//X軸計算
		D3DXVec3Normalize(&direction, &direction);

		//Y軸回転分だけ逆回転してローカル空間に変換
		D3DXMATRIX rotYInv;
		D3DXMatrixRotationY(&rotYInv, -m_vRotation.y);

		D3DXVECTOR3 localDir;
		D3DXVec3TransformNormal(&localDir, &direction, &rotYInv);

		float targetXAngle;

		//Y軸が±90以上の時反転を防ぐための処理
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

		//回転量を計算
		float rotAmountX = angleDiffX * min(M_RotSpeed, 1.0f);

		//角度を更新
		m_vRotation.x -= rotAmountX;

	}


}

float CBossMissile::Clamp(float value, float minVal, float maxVal)
{
	return (std::max)(minVal, (std::min)(maxVal, value));
}
