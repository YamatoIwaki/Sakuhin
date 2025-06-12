#include "CKaito.h"


CKaito::CKaito()
	: m_IsDebug			(false)
	
	, m_PlayerMotion	(enPlayerMotion::P_M_Stop)
	
	, m_MoveAround		(enMoveAround::Move_Around_Stop)
	, m_MoveAttendDance	(enMoveAttendDance::Move_Attend_Stop)
	, m_MoveSpeed		(0.1f)
	, m_MoveContSpeed	(1.0f)
	, m_IsAround		(false)
	, m_IsAttend		(false)
	, m_DefaAngle		()

	, m_RollingvecZ		()
	, m_RollingCoolTime	(RollingResetTime)

	, m_AttackCapsule	()

	, m_IsDie			(false)
{

	m_vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);

	m_AnimSpeed = AnimSpeed;
	
	m_Capsule.Radius = 0.5f;
	
	for (int i = 0; i < 3; i++)
	{
		m_AttackCapsule.push_back(CCapsuleCollision::Capsule{});

		m_AttackCapsule[i].Radius = (1.0f);

		m_AttackCapsule[i].HitJudg = false;
	}
}

CKaito::~CKaito()
{
}

void CKaito::Init()
{
	m_Scene = enScene::GameMain;

	m_Hp = MaxHp;
	ChangeMotion();

	m_vPosition = StartPos;

	m_IsDie = false;
}

void CKaito::Update()
{
	switch (m_Scene)
	{
	case Title:
	{
		if (m_AnimNo != enPlayerAnim::P_A_Wait)
		{
			m_AnimNo = enPlayerAnim::P_A_Wait;
			m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
		}

		m_vPosition = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
		m_vRotation.y = D3DXToRadian(180);

	}
		break;
	case GameMain:
	{
		if (m_IsDebug == true)
		{
			m_Hp = MaxHp;
		}


		if (m_PlayerMotion != enPlayerMotion::P_M_Die)
		{
			if (m_Hp <= 0.0f)
			{
				//HP���O�ȉ��ɂȂ����玀��
				CSoundManager::PlaySE(CSoundManager::SE_P_Die);
				ChangeMotion(enPlayerMotion::P_M_Die, enPlayerAnim::P_A_Dead, 0.003);
			}


			if (m_PlayerMotion != enPlayerMotion::P_M_Rolling)
			{
				//�N�[���^�C��
				m_RollingCoolTime++;
				if (GetAsyncKeyState(VK_SHIFT) & 0x8000 && m_RollingCoolTime >= RollingResetTime)
				{
					/*****************�ړ��v�Z********************/
					//Z���x�N�g��(z+�����ւ̒P�ʃx�N�g��)
					//������(�傫��)�@���P�̃x�N�g����P�ʃx�N�g���Ƃ���.
					m_RollingvecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

					//y�����̉�]�s��.
					D3DXMATRIX mRotationY;
					//Y����]�s����쐬.
					D3DXMatrixRotationY(
						&mRotationY,	//(out)�s��.
						m_QuaterRot.y);	//�v���C���[��Y�����̉�]�l.

					//Y����]�s����g����Z���x�N�g�������W�ω�����.
					D3DXVec3TransformCoord(
						&m_RollingvecZ,		//(out)Z���x�N�g��.
						&m_RollingvecZ,		//(in)Z���x�N�g��.
						&mRotationY);	//Y����]�s��.

					/********************************************/

					//��]���[�V�����ֈڍs
					ChangeMotion(enPlayerMotion::P_M_Rolling, enPlayerAnim::P_A_Rolling);
				}
			}
		}
		AttackRange();

		switch (m_PlayerMotion)
		{
		case CKaito::P_M_Stop:			//�ҋ@
		{
			if (GetAsyncKeyState('W') || GetAsyncKeyState('S'))
			{
				if (GetAsyncKeyState('W') && GetAsyncKeyState('S'))
				{ /*�O�ƌ�낪�Ԃ����Ă����珈�������Ȃ�*/
				}
				else
				{
					//�ړ����[�V������
					ChangeMotion(enPlayerMotion::P_M_Move, enPlayerAnim::P_A_Run);
				}
			}
			if (GetAsyncKeyState('A') || GetAsyncKeyState('D'))
			{
				if (GetAsyncKeyState('A') && GetAsyncKeyState('D'))
				{ /*���ƉE���Ԃ����Ă����珈�������Ȃ�*/
				}
				else
				{
					//�ړ����[�V������
					ChangeMotion(enPlayerMotion::P_M_Move, enPlayerAnim::P_A_Run);
				}
			}


			if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
			{
				CSoundManager::PlaySE(CSoundManager::SE_P_Sword1);
				m_AttackCapsule[P_C_Attack1].HitJudg = true;
				//�U�����[�V������
				ChangeMotion(enPlayerMotion::P_M_Attack1, enPlayerAnim::P_A_Attack1);
			}



		}
		break;
		case CKaito::P_M_Move:			//�ړ�
		{
			if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
			{
				m_AttackCapsule[P_C_Attack1].HitJudg = true;
				//�U�����[�V������
				ChangeMotion(enPlayerMotion::P_M_Attack1, enPlayerAnim::P_A_Attack1);
			}

			//�O
			if (GetAsyncKeyState('W') & 0x8000) {
				m_MoveAround = enMoveAround::Move_Forward;
			}
			//���.
			if (GetAsyncKeyState('S') & 0x8000) {
				if (GetAsyncKeyState('W') & 0x8000) {
					//�܂������Ă����Ԃ͈ړ���Ԃɂ���
					m_MoveAround = enMoveAround::Move_Around_Stop;	//�O�ƌ�낪���΂��Ă�����~�܂�
				}
				else {
					m_MoveAround = enMoveAround::Move_Backward;
				}
			}


			//�E
			if (GetAsyncKeyState('D') & 0x8000) {
				m_MoveAttendDance = enMoveAttendDance::Move_Rightward;
			}
			//��
			if (GetAsyncKeyState('A') & 0x8000) {
				if (GetAsyncKeyState('D') & 0x8000) {
					m_MoveAttendDance = enMoveAttendDance::Move_Attend_Stop; //�E�ƍ������΂��Ă�����~�܂�
				}
				else {
					m_MoveAttendDance = enMoveAttendDance::Move_Leftward;
				}
			}

			//�ǂ��������Ă��Ȃ���Αҋ@���[�V�����Ɉڂ�
			if (m_IsAround == false && m_IsAttend == false)
			{
				ChangeMotion();
			}
			//�΂߈ړ��ɂȂ�����R���g���[���l�����Ă�����
			else if (m_IsAround == true && m_IsAttend == true)
			{
				m_MoveContSpeed = MoveSlantCont;
			}
			else
			{
				m_MoveContSpeed = 1.0f;
			}
			MoveControl();
		}
		break;
		case CKaito::P_M_Rolling:		//��]�ړ�
		{

			m_AnimTime += m_AnimSpeed;

			//��]���Ȃ���ړ�
			m_vPosition += m_RollingvecZ * RollingSpeed;

			//true�ɂȂ��Ă���Ƃ������邽��
			for(int i = 0; i <= P_C_Attack3; i++){ 
				m_AttackCapsule[i].HitJudg = false;
			}

			if (m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Rolling) - 0.3 <= m_AnimTime)
			{
				m_RollingCoolTime = 0;
				ChangeMotion();
			}

		}
		break;

		case CKaito::P_M_Attack1:		//�U����i�K��
		{
			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Attack1) - 0.02 <= m_AnimTime)
			{
				m_AttackCapsule[P_C_Attack1].HitJudg = false;
				//�������Ȃ���Αҋ@���[�V����
				ChangeMotion();
			}
			else if (m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Attack1) - 0.8 <= m_AnimTime)
			{
				if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
				{
					CSoundManager::PlaySE(CSoundManager::SE_P_Sword2);
					m_AttackCapsule[P_C_Attack1].HitJudg = false;
					m_AttackCapsule[P_C_Attack2].HitJudg = true;
					//�U�����i�K�ֈڍs
					ChangeMotion(enPlayerMotion::P_M_Attack2, enPlayerAnim::P_A_Attack2);

				}
			}

		}
		break;
		case CKaito::P_M_Attack2:			//�U����i�K��
		{
			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Attack2) - 0.02 <= m_AnimTime)
			{
				m_AttackCapsule[P_C_Attack2].HitJudg = false;
				//�������Ȃ���Αҋ@���[�V����
				ChangeMotion();
			}
			else if (m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Attack2) - 0.8 <= m_AnimTime)
			{
				if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
				{
					CSoundManager::PlaySE(CSoundManager::SE_P_Sword3);
					m_AttackCapsule[P_C_Attack2].HitJudg = false;
					m_AttackCapsule[P_C_Attack3].HitJudg = true;
					//�U�����i�K�ֈڍs
					ChangeMotion(enPlayerMotion::P_M_Attack3, enPlayerAnim::P_A_Attack3);

				}
			}


		}
		break;

		case CKaito::P_M_Attack3:			//�U���O�i�K��
		{
			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Attack3) - 0.01 <= m_AnimTime)
			{
				m_AttackCapsule[P_C_Attack3].HitJudg = false;
				if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
				{
					m_AttackCapsule[P_C_Attack1].HitJudg = true;
					//�U�����i�K�֖߂�
					ChangeMotion(enPlayerMotion::P_M_Attack1, enPlayerAnim::P_A_Attack1);
				}
				else
				{
					//�������Ȃ���Αҋ@���[�V����
					ChangeMotion();
				}
			}
		}
		break;

		case CKaito::P_M_Die:
		{
			m_AnimTime += m_AnimSpeed;
			m_Hp = 0.0f;
			if (m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Dead) - 0.1 <= m_AnimTime)
			{
				m_AnimSpeed = 0;

				m_IsDie = true;
			}

		}
		break;

		}

		//�����蔻���ݒ�
		m_Capsule.Top = m_vPosition + D3DXVECTOR3(0.0f, 0.5f, 0.0f);
		m_Capsule.Bottom = m_vPosition;
	
	}
		break;
	case GameOver:
	{
		if (m_AnimNo != enPlayerAnim::P_A_Dead)
		{
			m_AnimNo = enPlayerAnim::P_A_Dead;
			m_pMesh->ChangeAnimSet_StartPos(m_AnimNo, m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Dead) - 0.1, m_pAnimCtrl);
			m_AnimSpeed = 0.0;

		}



		m_vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	}
		break;
	case GameClear:
	{
		if (m_AnimNo != enPlayerAnim::P_A_KnockBack)
		{
			m_AnimNo = enPlayerAnim::P_A_KnockBack;
			m_pMesh->ChangeAnimSet_StartPos(m_AnimNo, 0.15, m_pAnimCtrl);
			m_AnimSpeed = 0;

		}

		m_vRotation.y = D3DXToRadian(0);
		m_vRotation.x = D3DXToRadian(20);


		m_vPosition = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	}
		break;
	default:
		break;
	}

	//���C�̈ʒu���v���C���[�̍��W�ɂ��낦��.
	m_pRayY->Position = m_vPosition;
	//�n�ʂɂ߂荞�݉���̂��߁A�v���C���[�̈ʒu����������ɂ��Ă���.
	m_pRayY->Position.y += 0.3f;
	m_pRayY->RotationY = m_vRotation.y;

	//�\��(�O�㍶�E�ɐL�΂���)���C�̐ݒ�.
	for (int dir = 0; dir < CROSSRAY::max; dir++)
	{
		m_pCrossRay->Ray[dir].Position = m_vPosition;
		m_pCrossRay->Ray[dir].Position.y += 0.4f;
		m_pCrossRay->Ray[dir].RotationY = m_vRotation.y;
	}

}

void CKaito::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{

	m_pMesh->SetAnimSpeed(m_AnimSpeed);
	if (m_Scene == enScene::GameMain)
	{
		CSkinMeshObject::QuaterDraw(View, Proj, Light, Camera);
	}
	else
	{
		CSkinMeshObject::Draw(View, Proj, Light, Camera);
	}
}

//����
void CKaito::MoveControl()
{

	/*****************�ړ��v�Z********************/
	//Z���x�N�g��(z+�����ւ̒P�ʃx�N�g��)
	//������(�傫��)�@���P�̃x�N�g����P�ʃx�N�g���Ƃ���.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 vecAxisX(1.0f, 0.0f, 0.0f);

	D3DXVECTOR3 direction = CCamera::GetInstance()->GetPosition() - m_vPosition;

	m_DefaAngle = atan2f(direction.x, direction.z);

	//y�����̉�]�s��.
	D3DXMATRIX mRotationY;
	//Y����]�s����쐬.
	D3DXMatrixRotationY(
		&mRotationY,	//(out)�s��.
		m_DefaAngle - D3DXToRadian(180));	//�v���C���[��Y�����̉�]�l.

	//Y����]�s����g����Z���x�N�g�������W�ω�����.
	D3DXVec3TransformCoord(
		&vecAxisZ,		//(out)Z���x�N�g��.
		&vecAxisZ,		//(in)Z���x�N�g��.
		&mRotationY);	//Y����]�s��.

	//Y����]�s����g����Z���x�N�g�������W�ω�����.
	D3DXVec3TransformCoord(
		&vecAxisX,		//(out)Z���x�N�g��.
		&vecAxisX,		//(in)Z���x�N�g��.
		&mRotationY);	//Y����]�s��.
/********************************************/




	//�O��ړ�
	switch (m_MoveAround)
	{
	case CKaito::Move_Around_Stop:
		m_IsAround = false;
		break;
	case CKaito::Move_Forward:
		m_IsAround = true;
		m_vPosition += vecAxisZ * m_MoveSpeed * m_MoveContSpeed;
		break;
	case CKaito::Move_Backward:
		m_IsAround = true;
		m_vPosition -= vecAxisZ * m_MoveSpeed * m_MoveContSpeed;
		break;
	}

	//���E�ړ�
	switch (m_MoveAttendDance)
	{
	case CKaito::Move_Attend_Stop:
		m_IsAttend = false;
		break;
	case CKaito::Move_Rightward:
		m_IsAttend = true;
		m_vPosition += vecAxisX * m_MoveSpeed * m_MoveContSpeed;
		break;
	case CKaito::Move_Leftward:
		m_IsAttend = true;
		m_vPosition -= vecAxisX * m_MoveSpeed * m_MoveContSpeed;
		break;
	}
	
	m_MoveAround = enMoveAround::Move_Around_Stop;
	m_MoveAttendDance = enMoveAttendDance::Move_Attend_Stop; 


}

void CKaito::ChangeMotion(int Motion, int Animation, double ASpeed)
{
	m_AnimTime		= 0;
	m_AnimSpeed		= ASpeed;
	m_PlayerMotion	= Motion;
	m_AnimNo		= Animation;
	m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);

	if (Motion == enPlayerMotion::P_M_Stop)
	{
		ResetKeyStates();
	}

}

void CKaito::AttackRange()
{
	//Z���x�N�g��(z+�����ւ̒P�ʃx�N�g��)
	//������(�傫��)�@���P�̃x�N�g����P�ʃx�N�g���Ƃ���.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);

	//y�����̉�]�s��.
	D3DXMATRIX mRotationY;
	//Y����]�s����쐬.
	D3DXMatrixRotationY(
		&mRotationY,	//(out)�s��.
		m_QuaterRot.y);	//�v���C���[��Y�����̉�]�l.

	//Y����]�s����g����Z���x�N�g�������W�ω�����.
	D3DXVec3TransformCoord(
		&vecAxisZ,		//(out)Z���x�N�g��.
		&vecAxisZ,		//(in)Z���x�N�g��.
		&mRotationY);	//Y����]�s��.


	for (size_t n = 0; n < m_AttackCapsule.size(); n++)
	{
		m_AttackCapsule[n].Top = m_vPosition + (vecAxisZ * 1.5f);
		m_AttackCapsule[n].Bottom = m_vPosition + (vecAxisZ * 1.5f);
	}
}



