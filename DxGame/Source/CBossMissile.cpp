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
		//•\Ž¦ŽžŠÔ‚ð‰ÁŽZ
		m_ShotTime++;


		//Å‘å•\Ž¦ŽžŠÔ‚ð’´‚¦‚½‚çŽ~‚ß‚é
		if (m_ShotTime >= MaxAppeaTime)
		{
			m_ShotTime = 0;
			m_IsShot = false;
		}

		TargetDirection();

		DirectionMove();

	}
	

	//“–‚½‚è”»’è‚ðÝ’è
	m_Capsule.Top = m_vPosition;
	m_Capsule.Bottom = m_vPosition;

}

void CBossMissile::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	//”ò‚Î‚µ‚Ä‚¢‚éŽž‚¾‚¯•\Ž¦
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
	/*****************ƒxƒNƒgƒ‹ŒvŽZ********************/
//ZŽ²ƒxƒNƒgƒ‹(z+•ûŒü‚Ö‚Ì’PˆÊƒxƒNƒgƒ‹)
//¦’·‚³(‘å‚«‚³)@‚ª‚P‚ÌƒxƒNƒgƒ‹‚ð’PˆÊƒxƒNƒgƒ‹‚Æ‚¢‚¤.
	D3DXVECTOR3 vecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//y•ûŒü‚Ì‰ñ“]s—ñ.
	D3DXMATRIX mRotationX, mRotationY, mRotationZ;
	//YŽ²‰ñ“]s—ñ‚ðì¬.
	D3DXMatrixRotationX(&mRotationX, m_vRotation.x);
	D3DXMatrixRotationY(&mRotationY, m_vRotation.y);
	D3DXMatrixRotationZ(&mRotationZ, m_vRotation.z);

	D3DXMATRIX mRot = mRotationY * mRotationX * mRotationZ;

	//YŽ²‰ñ“]s—ñ‚ðŽg‚Á‚ÄZŽ²ƒxƒNƒgƒ‹‚ðÀ•W•Ï‰»‚·‚é.
	D3DXVec3TransformCoord(
		&vecZ,		//(out)ZŽ²ƒxƒNƒgƒ‹.
		&vecZ,		//(in)ZŽ²ƒxƒNƒgƒ‹.
		&mRot);	//YŽ²‰ñ“]s—ñ.

	/********************************************/

	//ˆÚ“®‚ð‰ÁŽZ
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

	//Œü‚«‚ÌXV
	if(m_IsHoming == true)
	{
		m_HomingTime++;

		D3DXVECTOR3 direction = CKaito::GetInstance()->GetPosition() - m_vPosition;
		
		//ƒ^[ƒQƒbƒg‚Ì^‚ñ’†‚ÉŒü‚©‚Á‚Ä‚Ù‚µ‚¢‚Ì‚Å­‚µã‚É‚·‚é
		direction.y += 0.5f;

		//YŽ²ŒvŽZ
		D3DXVECTOR3 dirFlat = direction;
		dirFlat.y = 0;

		//•ûŒü‚ªƒ[ƒ‚Ìê‡‚Í‰½‚à‚µ‚È‚¢
		if (D3DXVec3Length(&dirFlat) > 0.0004f)
		{

			//Šp“xŒvŽZ
			float targetAngle = atan2f(direction.x, direction.z);

			//Šp“x·‚ð‹‚ß‚é
			float angleDiffY = targetAngle - m_vRotation.y;

			while (angleDiffY > D3DX_PI)	angleDiffY -= D3DX_PI * 2.0f;
			while (angleDiffY < -D3DX_PI)	angleDiffY += D3DX_PI * 2.0f;

			//‰ñ“]—Ê‚ðŒvŽZ
			float rotAmountY = angleDiffY * min(M_RotSpeed, 1.0f);

			//Šp“x‚ðXV
			m_vRotation.y -= rotAmountY;
		}

		//XŽ²ŒvŽZ
		D3DXVec3Normalize(&direction, &direction);

		//YŽ²‰ñ“]•ª‚¾‚¯‹t‰ñ“]‚µ‚Äƒ[ƒJƒ‹‹óŠÔ‚É•ÏŠ·
		D3DXMATRIX rotYInv;
		D3DXMatrixRotationY(&rotYInv, -m_vRotation.y);

		D3DXVECTOR3 localDir;
		D3DXVec3TransformNormal(&localDir, &direction, &rotYInv);

		float targetXAngle;

		//YŽ²‚ª}90ˆÈã‚ÌŽž”½“]‚ð–h‚®‚½‚ß‚Ìˆ—
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

		//‰ñ“]—Ê‚ðŒvŽZ
		float rotAmountX = angleDiffX * min(M_RotSpeed, 1.0f);

		//Šp“x‚ðXV
		m_vRotation.x -= rotAmountX;

	}


}

float CBossMissile::Clamp(float value, float minVal, float maxVal)
{
	return (std::max)(minVal, (std::min)(maxVal, value));
}
