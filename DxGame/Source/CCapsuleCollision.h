#pragma once
#include "CDirectX9.h"

/**************************************
*		�J�v�Z�����m�̓����蔻��
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

	//�����߂�����
	static bool PushBack(Capsule& coll1, Capsule& coll2);
	//�������Ă��邩�ǂ����̏���
	static bool IsHit(const Capsule& coll1, const Capsule& coll2);
};

