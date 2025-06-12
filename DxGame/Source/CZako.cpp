#include "CZako.h"
#include "CKaito.h"

CZako::CZako()
	: m_IsShot			(false)

	, m_TacklePos		()
	, m_TackleLength	()
	, m_IsTackleMove	(false)
	, m_TackleTime		(0)
{
	m_vScale = D3DXVECTOR3(0.0015f, 0.0015f, 0.0015f);

	m_AnimSpeed = AnimWaitSpeed;
	m_AnimNo = enZakoAnimation::Z_A_Wait;

	m_Hp = MaxHp;

}

CZako::~CZako()
{
}


void CZako::Init()
{
}

void CZako::Update()
{
	switch (m_Scene)
	{
	case Title:
	{
		if (m_AnimNo != enZakoAnimation::Z_A_Wait)
		{
			m_AnimNo = enZakoAnimation::Z_A_Wait;
			m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
		}

	}
		break;
	case GameMain:
	{
		if (m_Hp <= 0 && m_Motion != enZakoMotion::Z_M_Die)
		{
			m_Hp = 0;

			CSoundManager::PlaySE(CSoundManager::SE_RoboDie);

			ChangeMotion(enZakoMotion::Z_M_Die, enZakoAnimation::Z_A_Appear);
		}


		//�v���C���[�Ƃ̈�苗���̏ꏊ��ݒ肷��
		TargetPosSet(RadiusOfTarget);

		if (m_Motion != enZakoMotion::Z_M_Tackle && m_Motion != enZakoMotion::Z_M_Die)
		{
			RotationToTarget(CKaito::GetInstance()->GetPosition());
		}
		//�ڕW�Ƃ̋���
		m_TargetLength = m_TargetPos - m_vPosition;


		switch (m_Motion)
		{
			//�ҋ@��
		case CZako::Z_M_Stop:
		{
			m_AttackMotionTime++;

			m_IsShot = false;

			//���ꂷ������ړ����[�V�����Ɉڂ�
			if (D3DXVec3Length(&m_TargetLength) > OverRadiusOfTarget)
			{
				ChangeMotion(enZakoMotion::Z_M_Move, enZakoAnimation::Z_A_Move);

			}
			else if (m_AttackMotionTime > MaxAttackCoolTime) {
				//���Ԃ��[���ɂ���
				m_AttackMotionTime = 0;
				//���[�V������I��
				ChoiceRandomMotion();
			}

		}
		break;
		//�ړ���
		case CZako::Z_M_Move:
		{
			//�ړ������Z����
			m_vPosition.x += (m_TargetPos.x - m_vPosition.x) * MoveSpeed;
			m_vPosition.z += (m_TargetPos.z - m_vPosition.z) * MoveSpeed;


			if (D3DXVec3Length(&m_TargetLength) <= 2.0f)
			{
				//�ڕW�̈ʒu�ɓ���������~�܂�
				ChangeMotion();
			}

		}
		break;
		//���ˏ�����
		case CZako::Z_M_Shot:
		{
			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 0.001)
			{
				m_AnimTime = 0;
				m_IsShot = true;

				//�ڕW���B��������~�܂�
				ChangeMotion();
			}


		}
		break;
		//�ːi�U��
		case CZako::Z_M_Tackle:
		{
			if (m_AnimSpeed > TackleReadySpeed)
			{
				CSoundManager::PlayLoop(CSoundManager::SE_Z_Tacle);

				m_TackleTime++;
				m_IsTackleMove = true;
				//�ړ������Z����
				m_vPosition.x += (m_TacklePos.x - m_vPosition.x) * TackleSpeed;
				m_vPosition.z += (m_TacklePos.z - m_vPosition.z) * TackleSpeed;

				m_TackleLength = m_TacklePos - m_vPosition;

				if (D3DXVec3Length(&m_TackleLength) <= 1.0f || m_TackleTime > MaxTackleTime)
				{
					CSoundManager::Stop(CSoundManager::SE_Z_Tacle);

					m_TackleTime = 0;
					m_IsTackleMove = false;
					m_Capsule.HitJudg = true;
					//�ڕW�̈ʒu�ɓ���������~�܂�
					ChangeMotion();
				}
			}
			else
			{
				m_TackleTime = 0;
				//�A�j���[�V�����X�s�[�h�����Z
				m_AnimSpeed += TackleAnimSumSpeed;
			}


		}
		break;
		//�����ł���
		case CZako::Z_M_Die:
		{
			m_vRotation.y += D3DXToRadian(DieRotSpeed);

			m_vScale -= D3DXVECTOR3(DieSmallSpeed, DieSmallSpeed, DieSmallSpeed);

			m_Capsule.HitJudg = false;
			if (m_vScale.y <= DieMinSmallSize)
			{
				m_IsDie = true;
			}
		}
		break;

		}


		//�����蔻���ݒ�
		m_Capsule.Top = m_vPosition + D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_Capsule.Bottom = m_vPosition + D3DXVECTOR3(0.0f, 1.0f, 0.0f);


	
	}
		break;
	case GameOver:
	{
	
		if (m_AnimNo != enZakoAnimation::Z_A_Move)
		{
			m_AnimNo = enZakoAnimation::Z_A_Move;
			m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
		}

		//�ڕW�֌���
		RotationToTarget(CKaito::GetInstance()->GetPosition(), D3DXToRadian(0));

		D3DXVECTOR3 vecAxisX(1.0f, 0.0f, 0.0f);

		//y�����̉�]�s��.
		D3DXMATRIX mRotationY;
		//Y����]�s����쐬.
		D3DXMatrixRotationY(
			&mRotationY,	//(out)�s��.
			m_vRotation.y);	//�v���C���[��Y�����̉�]�l.

		//Y����]�s����g����Z���x�N�g�������W�ω�����.
		D3DXVec3TransformCoord(
			&vecAxisX,		//(out)Z���x�N�g��.
			&vecAxisX,		//(in)Z���x�N�g��.
			&mRotationY);	//Y����]�s��.


		m_vPosition += vecAxisX * 0.04f;

	}
		break;
	case GameClear:
	{
		if (m_AnimNo != enZakoAnimation::Z_A_Appear)
		{
			m_AnimNo = enZakoAnimation::Z_A_Appear;
			m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
			m_AnimSpeed = 0.0;
		}

	
		if (!m_IfRandOfGC)
		{
			m_IfRandOfGC = true;

			std::random_device rd;  
			std::mt19937 gen(rd()); 
			std::uniform_int_distribution<> dist(-90, 90);

			int randomValue = dist(gen);

			m_vRotation.z = D3DXToRadian(randomValue);

		}
	}
		break;
	}


}

void CZako::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if (m_IsDie == false)
	{
		m_pMesh->SetAnimSpeed(m_AnimSpeed);
		CEnemy::Draw(View, Proj, Light, Camera);
	}

}

void CZako::DrawShadowMap()
{
	if (m_IsDie == false)
	{
		CEnemy::DrawShadowMap();
	}
}

void CZako::RayUpdate()
{
	CEnemy::RayUpdate();
}

void CZako::ChangeMotion(int Motion, int Animation, double ASpeed)
{
	m_AnimSpeed = ASpeed;
	m_Motion = Motion;
	m_AnimNo = Animation;
	m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
}

void CZako::TacklePosSet()
{
	m_PlayerLength = CKaito::GetInstance()->GetPosition() - m_vPosition;

	D3DXVec3Normalize(&m_PlayerLength, &m_PlayerLength);

	m_TacklePos = CKaito::GetInstance()->GetPosition() + m_PlayerLength * 5.0f;

}

void CZako::ChoiceRandomMotion()
{
	//���A�E�A���̕���������
	std::vector<std::string> choices
		= { "�V���b�g", "�^�b�N��" };

	//�e�I�����̊m����ݒ�
	std::vector<double> weights = { 0.7, 0.3 };

	//�����_���̏���
	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> dist(weights.begin(), weights.end());

	//�����_���I��
	int index = dist(gen);

	CSoundManager::PlaySE(CSoundManager::SE_B_Sride);

	//�I�����ꂽ���̕ʂɏ���������
	if (choices[index] == "�V���b�g")
	{
		//�V���b�g���[�V����
		ChangeMotion(enZakoMotion::Z_M_Shot, Z_A_Appear, AnimApperSpeed);
	}
	else if (choices[index] == "�^�b�N��")
	{
		//�ːi���[�V����
		ChangeMotion(enZakoMotion::Z_M_Tackle, Z_A_Move, AnimMoveSpeed);
		TacklePosSet();
	}

}

