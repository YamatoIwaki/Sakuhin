#include "CCapsuleCollision.h"

CCapsuleCollision::CCapsuleCollision()
{
}

CCapsuleCollision::~CCapsuleCollision()
{
}

bool CCapsuleCollision::PushBack(Capsule& coll1, Capsule& coll2)
{
	//当たり判定をつけていないと
	if (coll1.HitJudg == false || coll2.HitJudg == false)
	{
		return false;
	}

	//各カプセルの中心線ベクトルを計算
	D3DXVECTOR3 d1 = coll1.Bottom - coll1.Top;
	D3DXVECTOR3 d2 = coll2.Bottom - coll2.Top;
	D3DXVECTOR3 startDiff = coll1.Top - coll2.Top;

	//各中心線ベクトルの長さの2乗を計算
	float thisDirectionDot = D3DXVec3Dot(&d1, &d1);
	float otherDirectionDot = D3DXVec3Dot(&d2, &d2);
	float otherDotStartDiff = D3DXVec3Dot(&d2, &d1);

	float thisParam, otherParam;

	//特殊ケース処理
	if (thisDirectionDot <= 1e-6f && otherDirectionDot <= 1e-6f)
	{
		//両方のカプセルが点の場合
		thisParam = otherParam = 0.0f;
	}
	else if (thisDirectionDot <= 1e-6f)
	{
		//このカプセルが点の場合
		thisParam = 0.0f;
		otherParam = std::clamp(otherDotStartDiff / otherDirectionDot, 0.0f, 1.0f);
	}
	else
	{
		float thisDotStartDiff = D3DXVec3Dot(&d1, &startDiff);
		if (otherDirectionDot <= 1e-6f)
		{
			//対象のカプセルが点の場合
			otherParam = 0.0f;
			thisParam = std::clamp(-thisDotStartDiff / thisDirectionDot, 0.0f, 1.0f);
		}
		else
		{
			//一般ケース
			//DotProductBetweenDirectinsの略
			float dpbd = D3DXVec3Dot(&d1, &d2);
			float denomi = thisDirectionDot * otherDirectionDot - dpbd * dpbd;

			if (denomi != 0.0f)
			{
				thisParam = std::clamp((dpbd * otherDotStartDiff - thisDotStartDiff * otherDirectionDot)
					/ denomi, 0.0f, 1.0f);
			}
			else
			{
				thisParam = 0.0f;
			}

			otherParam = std::clamp((dpbd * thisParam * otherDotStartDiff) / otherDirectionDot, 0.0f, 1.0f);
		}
	}

	//最接近接点を計算し、2転換の距離を求める
	D3DXVECTOR3 ThisCapsule = coll1.Top + thisParam * d1;
	D3DXVECTOR3 OtherCapsule = coll2.Top + otherParam * d2;
	D3DXVECTOR3 m_Diff = ThisCapsule - OtherCapsule;


	//距離の2乗がカプセルの半径の合計の2乗以下なら衝突
	float DistSquared = D3DXVec3Dot(&m_Diff, &m_Diff);
	float RadiusSum = coll1.Radius + coll2.Radius;


	//衝突している
	if (DistSquared <= RadiusSum * RadiusSum)
	{
		float distance = std::sqrt(DistSquared);
		float penetrationDepth = RadiusSum - distance;

		//押し戻しベクトルを計算
		D3DXVECTOR3 pushVec = (distance > 1e-6f) ? m_Diff / distance : D3DXVECTOR3(1.f, 0.0f, 0.0f);


		//カプセルを押し戻す
		coll1.Translate(coll1, penetrationDepth * pushVec);
		coll2.Translate(coll2, penetrationDepth * -pushVec);

		return true;
	}
	return false;
}

bool CCapsuleCollision::IsHit(const Capsule& coll1, const Capsule& coll2)
{
	//当たり判定をつけていないと
	if (coll1.HitJudg == false || coll2.HitJudg == false)
	{
		return false;
	}

	//各カプセルの中心線ベクトルを計算
	D3DXVECTOR3 d1 = coll1.Bottom - coll1.Top;
	D3DXVECTOR3 d2 = coll2.Bottom - coll2.Top;
	D3DXVECTOR3 startDiff = coll1.Top - coll2.Top;

	//各中心線ベクトルの長さの2乗を計算
	float thisDirectionDot = D3DXVec3Dot(&d1, &d1);
	float otherDirectionDot = D3DXVec3Dot(&d2, &d2);
	float otherDotStartDiff = D3DXVec3Dot(&d2, &d1);

	float thisParam, otherParam;

	//特殊ケース処理
	if (thisDirectionDot <= 1e-6f && otherDirectionDot <= 1e-6f)
	{
		//両方のカプセルが点の場合
		thisParam = otherParam = 0.0f;
	}
	else if (thisDirectionDot <= 1e-6f)
	{
		//このカプセルが点の場合
		thisParam = 0.0f;
		otherParam = std::clamp(otherDotStartDiff / otherDirectionDot, 0.0f, 1.0f);
	}
	else
	{
		float thisDotStartDiff = D3DXVec3Dot(&d1, &startDiff);
		if (otherDirectionDot <= 1e-6f)
		{
			//対象のカプセルが点の場合
			otherParam = 0.0f;
			thisParam = std::clamp(-thisDotStartDiff / thisDirectionDot, 0.0f, 1.0f);
		}
		else
		{
			//一般ケース
			//DotProductBetweenDirectinsの略
			float dpbd = D3DXVec3Dot(&d1, &d2);
			float denomi = thisDirectionDot * otherDirectionDot - dpbd * dpbd;

			if (denomi != 0.0f)
			{
				thisParam = std::clamp((dpbd * otherDotStartDiff - thisDotStartDiff * otherDirectionDot)
					/ denomi, 0.0f, 1.0f);
			}
			else
			{
				thisParam = 0.0f;
			}

			otherParam = std::clamp((dpbd * thisParam * otherDotStartDiff) / otherDirectionDot, 0.0f, 1.0f);
		}
	}

	//最接近接点を計算し、2転換の距離を求める
	D3DXVECTOR3 ThisCapsule = coll1.Top + thisParam * d1;
	D3DXVECTOR3 OtherCapsule = coll2.Top + otherParam * d2;
	D3DXVECTOR3 m_Diff = ThisCapsule - OtherCapsule;


	//距離の2乗がカプセルの半径の合計の2乗以下なら衝突
	float DistSquared = D3DXVec3Dot(&m_Diff, &m_Diff);
	float RadiusSum = coll1.Radius + coll2.Radius;


	//衝突している
	if (DistSquared <= RadiusSum * RadiusSum)
	{
		return true;
	}
	return false;
}



