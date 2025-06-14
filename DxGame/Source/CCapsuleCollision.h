#pragma once
#include "CDirectX9.h"

/**************************************
*		カプセル同士の当たり判定
**/

class CCapsuleCollision
{
public:


	struct Capsule
	{
		D3DXVECTOR3			Top;
		D3DXVECTOR3			Bottom;
		float				Radius = 0.5f;
		bool				HitJudg = true;


		void Translate(Capsule& cap, const D3DXVECTOR3& offset)
		{
			cap.Top += offset;
			cap.Bottom += offset;
		}

		void SetHitJudg(bool hit)
		{
			HitJudg = hit;
		}
	};


public:
	CCapsuleCollision();
	~CCapsuleCollision();

	//押し戻し処理
	static bool PushBack(Capsule& coll1, Capsule& coll2);
	//当たっているかどうかの処理
	static bool IsHit(const Capsule& coll1, const Capsule& coll2);
};

