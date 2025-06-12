#include "CCapsuleCollision.h"

CCapsuleCollision::CCapsuleCollision()
{
}

CCapsuleCollision::~CCapsuleCollision()
{
}

bool CCapsuleCollision::PushBack(Capsule& coll1, Capsule& coll2)
{
	//�����蔻������Ă��Ȃ���
	if (coll1.HitJudg == false || coll2.HitJudg == false)
	{
		return false;
	}

	//�e�J�v�Z���̒��S���x�N�g�����v�Z
	D3DXVECTOR3 d1 = coll1.Bottom - coll1.Top;
	D3DXVECTOR3 d2 = coll2.Bottom - coll2.Top;
	D3DXVECTOR3 startDiff = coll1.Top - coll2.Top;

	//�e���S���x�N�g���̒�����2����v�Z
	float thisDirectionDot = D3DXVec3Dot(&d1, &d1);
	float otherDirectionDot = D3DXVec3Dot(&d2, &d2);
	float otherDotStartDiff = D3DXVec3Dot(&d2, &d1);

	float thisParam, otherParam;

	//����P�[�X����
	if (thisDirectionDot <= 1e-6f && otherDirectionDot <= 1e-6f)
	{
		//�����̃J�v�Z�����_�̏ꍇ
		thisParam = otherParam = 0.0f;
	}
	else if (thisDirectionDot <= 1e-6f)
	{
		//���̃J�v�Z�����_�̏ꍇ
		thisParam = 0.0f;
		otherParam = std::clamp(otherDotStartDiff / otherDirectionDot, 0.0f, 1.0f);
	}
	else
	{
		float thisDotStartDiff = D3DXVec3Dot(&d1, &startDiff);
		if (otherDirectionDot <= 1e-6f)
		{
			//�Ώۂ̃J�v�Z�����_�̏ꍇ
			otherParam = 0.0f;
			thisParam = std::clamp(-thisDotStartDiff / thisDirectionDot, 0.0f, 1.0f);
		}
		else
		{
			//��ʃP�[�X
			//DotProductBetweenDirectins�̗�
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

	//�Őڋߐړ_���v�Z���A2�]���̋��������߂�
	D3DXVECTOR3 ThisCapsule = coll1.Top + thisParam * d1;
	D3DXVECTOR3 OtherCapsule = coll2.Top + otherParam * d2;
	D3DXVECTOR3 m_Diff = ThisCapsule - OtherCapsule;


	//������2�悪�J�v�Z���̔��a�̍��v��2��ȉ��Ȃ�Փ�
	float DistSquared = D3DXVec3Dot(&m_Diff, &m_Diff);
	float RadiusSum = coll1.Radius + coll2.Radius;


	//�Փ˂��Ă���
	if (DistSquared <= RadiusSum * RadiusSum)
	{
		float distance = std::sqrt(DistSquared);
		float penetrationDepth = RadiusSum - distance;

		//�����߂��x�N�g�����v�Z
		D3DXVECTOR3 pushVec = (distance > 1e-6f) ? m_Diff / distance : D3DXVECTOR3(1.f, 0.0f, 0.0f);


		//�J�v�Z���������߂�
		coll1.Translate(coll1, penetrationDepth * pushVec);
		coll2.Translate(coll2, penetrationDepth * -pushVec);

		return true;
	}
	return false;
}

bool CCapsuleCollision::IsHit(const Capsule& coll1, const Capsule& coll2)
{
	//�����蔻������Ă��Ȃ���
	if (coll1.HitJudg == false || coll2.HitJudg == false)
	{
		return false;
	}

	//�e�J�v�Z���̒��S���x�N�g�����v�Z
	D3DXVECTOR3 d1 = coll1.Bottom - coll1.Top;
	D3DXVECTOR3 d2 = coll2.Bottom - coll2.Top;
	D3DXVECTOR3 startDiff = coll1.Top - coll2.Top;

	//�e���S���x�N�g���̒�����2����v�Z
	float thisDirectionDot = D3DXVec3Dot(&d1, &d1);
	float otherDirectionDot = D3DXVec3Dot(&d2, &d2);
	float otherDotStartDiff = D3DXVec3Dot(&d2, &d1);

	float thisParam, otherParam;

	//����P�[�X����
	if (thisDirectionDot <= 1e-6f && otherDirectionDot <= 1e-6f)
	{
		//�����̃J�v�Z�����_�̏ꍇ
		thisParam = otherParam = 0.0f;
	}
	else if (thisDirectionDot <= 1e-6f)
	{
		//���̃J�v�Z�����_�̏ꍇ
		thisParam = 0.0f;
		otherParam = std::clamp(otherDotStartDiff / otherDirectionDot, 0.0f, 1.0f);
	}
	else
	{
		float thisDotStartDiff = D3DXVec3Dot(&d1, &startDiff);
		if (otherDirectionDot <= 1e-6f)
		{
			//�Ώۂ̃J�v�Z�����_�̏ꍇ
			otherParam = 0.0f;
			thisParam = std::clamp(-thisDotStartDiff / thisDirectionDot, 0.0f, 1.0f);
		}
		else
		{
			//��ʃP�[�X
			//DotProductBetweenDirectins�̗�
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

	//�Őڋߐړ_���v�Z���A2�]���̋��������߂�
	D3DXVECTOR3 ThisCapsule = coll1.Top + thisParam * d1;
	D3DXVECTOR3 OtherCapsule = coll2.Top + otherParam * d2;
	D3DXVECTOR3 m_Diff = ThisCapsule - OtherCapsule;


	//������2�悪�J�v�Z���̔��a�̍��v��2��ȉ��Ȃ�Փ�
	float DistSquared = D3DXVec3Dot(&m_Diff, &m_Diff);
	float RadiusSum = coll1.Radius + coll2.Radius;


	//�Փ˂��Ă���
	if (DistSquared <= RadiusSum * RadiusSum)
	{
		return true;
	}
	return false;
}



