#include "CBoss.h"

CBoss::CBoss()
	: m_IsApper			()
	, m_StepCoolTime	()
	, m_StepTime		()
	, m_IsMissile		()
	, m_IsSetMissile	()
	, m_IsSAttack		()
	, m_ChangeSAttack	()
{
	m_vScale *= 0.03f;

}

CBoss::~CBoss()
{
}

void CBoss::Init()
{
	m_Scene		= enScene::GameMain;

	m_Hp		= MaxHp;
	ChangeMotion(B_M_Boot, B_A_Boot, 0.003);

	m_vPosition = D3DXVECTOR3(0.0f, 0.0f, 10.0f);

	m_IsDie		= false;

	m_IsApper	= true;
}

void CBoss::Update()
{
	switch (m_Scene)
	{
	case Title:
	{
		if (m_AnimNo != enBossAnimation::B_A_Wait)
		{
			m_AnimNo = enBossAnimation::B_A_Wait;
			m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
		}

		m_vPosition.x = 0.0f;
		m_vPosition.z = 20.0f;
		m_vPosition.y = 5.0f;

		m_vRotation.y = D3DXToRadian(0);
		m_vRotation.z = D3DXToRadian(0);

	}
		break;
	case GameMain:
	{
		//Hp���O�ɂȂ�Ǝ��ʃ��[�V�����ɓ���
		if (m_Hp <= 0.0f && m_Motion != enBossMotion::B_M_Die)
		{
			CSoundManager::PlaySE(CSoundManager::SE_B_Die);
			ChangeMotion(B_M_Die, B_A_BeginLose);
		}

		//�v���C���[�Ƃ̈�苗���̏ꏊ��ݒ肷��
		if (m_Motion != B_M_SwordAttack && m_Motion != B_M_Die && m_Motion != B_M_Boot)
		{
			TargetPosSet(RadiusOfTargetDefalt);
			RotationToTarget(CKaito::GetInstance()->GetPosition());
		}
		
		//�ڕW�Ƃ̋���
		m_TargetLength = m_TargetPos - m_vPosition;

		D3DXVECTOR3 vecAxisX(1.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);

		//y�����̉�]�s��.
		D3DXMATRIX mRotationY;
		//Y����]�s����쐬.
		D3DXMatrixRotationY(
			&mRotationY,	//(out)�s��.
			m_vRotation.y);	//�v���C���[��Y�����̉�]�l.

		//Y����]�s����g����Z���x�N�g�������W�ω�����.
		D3DXVec3TransformCoord(
			&vecAxisX,		//(out)X���x�N�g��.
			&vecAxisX,		//(in)X���x�N�g��.
			&mRotationY);	//Y����]�s��.

		//Y����]�s����g����Z���x�N�g�������W�ω�����.
		D3DXVec3TransformCoord(
			&vecAxisZ,		//(out)Z���x�N�g��.
			&vecAxisZ,		//(in)Z���x�N�g��.
			&mRotationY);	//Y����]�s��.


		switch (m_Motion)
		{
		case CBoss::B_M_Boot:
		{
			m_vPosition.z -= 0.02f;

			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(B_A_Boot) < m_AnimTime)
			{
				ChangeMotion();
			}



		}
			break;
		case CBoss::B_M_Wait:
		{
			m_StepCoolTime++;
			m_AttackMotionTime++;

			//���ꂷ������ړ����[�V�����Ɉڂ�
			if (D3DXVec3Length(&m_TargetLength) > 8.0f)
			{
				ChangeMotion(enBossMotion::B_M_Straight, enBossAnimation::B_A_BeginStraight);
			}
			else if (m_StepCoolTime >= StepCoolTime)
			{
				m_StepCoolTime = 0;
				WhatStep();
			}
			if (m_AttackMotionTime >= AttackCoolTime)
			{
				m_AttackMotionTime = 0;
				WhatAttack();
			}


		}
			break;
		case CBoss::B_M_Straight:
		{
			//�ړ������Z����
			m_vPosition.x += (m_TargetPos.x - m_vPosition.x) * MoveSpeed;
			m_vPosition.z += (m_TargetPos.z - m_vPosition.z) * MoveSpeed;
			
			if (m_AnimNo == enBossAnimation::B_A_BeginStraight)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 0.01)
				{
					//���M���߂��I�������ړ����A�j���[�V�����ɂ���
					m_AnimNo = enBossAnimation::B_A_Straight;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (D3DXVec3Length(&m_TargetLength) <= 2.0f)
			{
				if (m_AnimNo == enBossAnimation::B_A_Straight)
				{
					//�ڕW�̈ʒu�ɓ���������~�܂�A�j���[�V�����ɂ���
					m_AnimNo = enBossAnimation::B_A_EndStraight;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_AnimNo == enBossAnimation::B_A_EndStraight)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime)
				{
					//�~�܂�A�j���[�V�������I�������ҋ@��Ԃɂ���
					ChangeMotion();
				}
			}
		}
			break;
		case CBoss::B_M_RightStep:
		{
			m_vPosition -= vecAxisX * StepSpeed;

			m_StepTime++;

			if (m_AnimNo == enBossAnimation::B_A_BeginRightStep)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 0.01)
				{
					//���M���߂��I�������ړ����A�j���[�V�����ɂ���
					m_AnimNo = enBossAnimation::B_A_RightStep;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_StepTime >= MaxStepTime)
			{
				if (m_AnimNo == enBossAnimation::B_A_RightStep)
				{
					//�ڕW�̈ʒu�ɓ���������~�܂�A�j���[�V�����ɂ���
					m_AnimNo = enBossAnimation::B_A_EndRightStep;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_AnimNo == enBossAnimation::B_A_EndRightStep)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime)
				{
					//�~�܂�A�j���[�V�������I�������ҋ@��Ԃɂ���
					ChangeMotion();
					m_StepTime = 0;
				}
			}
		}
			break;
		case CBoss::B_M_LeftStep:
		{
			m_vPosition += vecAxisX * StepSpeed;

			m_StepTime++;

			if (m_AnimNo == enBossAnimation::B_A_BeginLeftStep)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 0.01)
				{
					//���M���߂��I�������ړ����A�j���[�V�����ɂ���
					m_AnimNo = enBossAnimation::B_A_LeftStep;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_StepTime >= MaxStepTime)
			{
				if (m_AnimNo == enBossAnimation::B_A_LeftStep)
				{
					//�ڕW�̈ʒu�ɓ���������~�܂�A�j���[�V�����ɂ���
					m_AnimNo = enBossAnimation::B_A_EndLeftStep;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_AnimNo == enBossAnimation::B_A_EndLeftStep)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime)
				{
					//�~�܂�A�j���[�V�������I�������ҋ@��Ԃɂ���
					ChangeMotion();
					m_StepTime = 0;
				}
			}

		}
			break;
		case CBoss::B_M_BackStep:
		{
			m_vPosition += vecAxisZ * StepSpeed;

			m_StepTime++;

			if (m_AnimNo == enBossAnimation::B_A_BeginBackStep)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 0.01)
				{
					//���M���߂��I�������ړ����A�j���[�V�����ɂ���
					m_AnimNo = enBossAnimation::B_A_BackStep;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_StepTime >= MaxStepTime)
			{
				if (m_AnimNo == enBossAnimation::B_A_BackStep)
				{
					//�ڕW�̈ʒu�ɓ���������~�܂�A�j���[�V�����ɂ���
					m_AnimNo = enBossAnimation::B_A_EndBackStep;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_AnimNo == enBossAnimation::B_A_EndBackStep)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime)
				{
					//�~�܂�A�j���[�V�������I�������~�T�C���U��������
					ChangeMotion(enBossMotion::B_M_Missile, enBossAnimation::B_A_Missile);
					m_StepTime = 0;
				}
			}

		}
			break;
		case CBoss::B_M_SwordAttack:
		{
			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 0.85)
			{
				//���̐U��n��(��x�����؂�ւ���)
				if (m_ChangeSAttack == false)
				{
					CSoundManager::PlaySE(CSoundManager::SE_B_Sword);

					m_ChangeSAttack = true;
					m_IsSAttack = true;
				}
			}

			if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 0.01)
			{
				m_ChangeSAttack = false;
				m_IsSAttack = false;
				//�U������x������ҋ@��Ԃɖ߂�
				ChangeMotion();
			}

			//�߂��Ȃ�܂ł����Z����
			m_vPosition.x += (m_TargetPos.x - m_vPosition.x) * SordAttackMoveSpeed;
			m_vPosition.z += (m_TargetPos.z - m_vPosition.z) * SordAttackMoveSpeed;

		}
			break;
		case CBoss::B_M_Missile:
		{
			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 1.0)
			{
				//�Z�b�g�����������甭�˂̍��}�𑗂�
				if (m_IsSetMissile == true)
				{
					m_IsMissile		= true;
					m_IsSetMissile	= false;
				}
			}
			else if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 1.5)
			{
				m_IsSetMissile = true;
			}

			if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 0.01)
			{
				//�A�j���[�V�������Ō�܂ł�����ҋ@��Ԃɖ߂�
				ChangeMotion();
			}
		}
			break;
		case CBoss::B_M_Die:
		{
			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime)
			{
				m_AnimNo = enBossAnimation::B_A_Lose;
				m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				m_IsDie = true;
			}

			m_Hp = 0.0f;

		}
			break;
		default:
			break;
		}

		//�����蔻���ݒ�
		m_Capsule.Radius = 1.0f;
		m_Capsule.Top = m_vPosition + D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_Capsule.Bottom = m_vPosition + D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	}
		break;
	case GameOver:
	{
		if (m_AnimNo != enBossAnimation::B_A_Wait)
		{
			m_AnimNo = enBossAnimation::B_A_Wait;
			m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
		}

		m_vPosition.x = 0.0f;
		m_vPosition.y = 5.0f;
		m_vPosition.z = 20.0f;

	}
		break;
	case GameClear:
	{

		if (m_AnimNo != enBossAnimation::B_A_Wait)
		{
			m_AnimNo = enBossAnimation::B_A_Wait;
			m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
			m_AnimSpeed = 0.0;
		}

		m_vPosition.x = -2.0f;
		m_vPosition.y = 4.0f;
		m_vPosition.z = 0.0f;


		m_vRotation.y = D3DXToRadian(90);
		m_vRotation.z = D3DXToRadian(-90);



	}
		break;
	default:
		break;
	}




}

void CBoss::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	m_pMesh->SetAnimSpeed(m_AnimSpeed);
	CEnemy::Draw(View, Proj, Light, Camera);
}

void CBoss::DrawShadowMap()
{
	CEnemy::DrawShadowMap();
}

void CBoss::RayUpdate()
{
	CEnemy::RayUpdate();
}

void CBoss::ChangeMotion(int Motion, int Animation, double ASpeed)
{
	m_AnimTime	= 0;
	m_AnimSpeed = ASpeed;
	m_Motion	= Motion;
	m_AnimNo	= Animation;
	m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
}

void CBoss::WhatStep()
{
	//���A�E�A���̕���������
	std::vector<std::string> choices 
		= { "�E", "��", "���"};

	//�e�I�����̊m����ݒ�
	std::vector<double> weights = { 0.4, 0.4, 0.2 };

	//�����_���̏���
	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> dist(weights.begin(), weights.end());

	//�����_���I��
	int index = dist(gen);

	CSoundManager::PlaySE(CSoundManager::SE_B_Sride);

	//�I�����ꂽ���̕ʂɏ���������
	if (choices[index] == "�E")
	{
		ChangeMotion(enBossMotion::B_M_RightStep, enBossAnimation::B_A_BeginRightStep);
	}
	else if (choices[index] == "��")
	{
		ChangeMotion(enBossMotion::B_M_LeftStep, enBossAnimation::B_A_BeginLeftStep);
	}
	else if (choices[index] == "���")
	{
		ChangeMotion(enBossMotion::B_M_BackStep, enBossAnimation::B_A_BeginBackStep);
	}

}

void CBoss::WhatAttack()
{
	//���ƃ~�T�C��������
	std::vector<std::string> choices
		= { "��", "�~�T�C��" };

	//�I�����̊m���������ɂ���ĕς��悤�ݒ�
#if true

	D3DXVECTOR3 length = CKaito::GetInstance()->GetPosition() - m_vPosition;

	std::vector<double> weights = CulculateWeightsFromDistance(D3DXVec3Length(&length));
#else
	std::vector<double> weights = { 1.0, 0.0};
#endif

	//�����_���̏���
	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> dist(weights.begin(), weights.end());

	//�����_���I��
	int index = dist(gen);

	if (choices[index] == "��")
	{
		//���U���ɓ���ۋ߂Â��悤�ڕW�ʒu���Z�b�g����

		TargetPosSet(RadiusOfSwordAttack);
		ChangeMotion(enBossMotion::B_M_SwordAttack, enBossAnimation::B_A_SwordAttack);
	}
	else if (choices[index] == "�~�T�C��")
	{
		ChangeMotion(enBossMotion::B_M_Missile, enBossAnimation::B_A_Missile);
	}

}

std::vector<double> CBoss::CulculateWeightsFromDistance(float distance)
{
	//������0~20�ŕω�����
	float nearWeight	= 1.0f / (distance + 0.1f);		//�ߋ����U��(�ŏ��l��0.0f�ɂȂ�Ȃ��悤��+1.0f������)

	float farWeight	= distance / MaxAttackLength;				//�������U��

	//double�^�ɕϊ����ĕԂ�
	return {static_cast<double>(nearWeight),
			static_cast<double>(farWeight)};

}
