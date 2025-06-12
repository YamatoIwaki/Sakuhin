#include "CZako2.h"

CZako2::CZako2()
	: m_IsAttack	(false)
	, m_SrideTime	()
{
	m_vRotation.y = D3DXToRadian(180);
	m_vScale *= 2.0f;
	m_AnimSpeed = AnimSpeed;

	m_Hp = MaxHp;
}

CZako2::~CZako2()
{
}

void CZako2::Init()
{
}

void CZako2::Update()
{
	switch (m_Scene)
	{
	case Title:
	{
		if (m_AnimNo != enZako2Animation::Z2_A_Move)
		{
			m_AnimNo = enZako2Animation::Z2_A_Move;
			m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
		}


	}
		break;
	case GameMain:
	{
		if (m_Hp <= 0 && m_Motion != enZako2Motion::Z2_M_Die)
		{
			m_Hp = 0;
			m_IsAttack = false;

			CSoundManager::PlaySE(CSoundManager::SE_RoboDie);

			//目標の位置に到着したら止まる
			ChangeMotion(enZako2Motion::Z2_M_Die, enZako2Animation::Z2_A_Jump, 0.0);
		}


		//プレイヤーとの一定距離の場所を設定する
		TargetPosSet(RadiusOfTarget);

		if (m_Motion != enZako2Motion::Z2_M_Attack && m_Motion != enZako2Motion::Z2_M_Die)
		{
			//目標へ向く
			RotationToTarget(CKaito::GetInstance()->GetPosition(), D3DXToRadian(180));
		}

		//目標との距離
		m_TargetLength = m_TargetPos - m_vPosition;

		D3DXVECTOR3 vecAxisX(1.0f, 0.0f, 0.0f);

		//y方向の回転行列.
		D3DXMATRIX mRotationY;
		//Y軸回転行列を作成.
		D3DXMatrixRotationY(
			&mRotationY,	//(out)行列.
			m_vRotation.y);	//プレイヤーのY方向の回転値.

		//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
		D3DXVec3TransformCoord(
			&vecAxisX,		//(out)Z軸ベクトル.
			&vecAxisX,		//(in)Z軸ベクトル.
			&mRotationY);	//Y軸回転行列.


		switch (m_Motion)
		{
		case enZako2Motion::Z2_M_Stop:
		{
			m_AttackMotionTime++;

			//離れすぎたら移動モーションに移る
			if (D3DXVec3Length(&m_TargetLength) > OverRadiusOfTarget)
			{
				ChangeMotion(enZako2Motion::Z2_M_Move, enZako2Animation::Z2_A_StreetSride, AnimSpeed);
				
			}
			else if(m_AttackMotionTime > MaxAttackCoolTime)
			{
				m_AttackMotionTime = 0;
				ChoiceRandomMotion();

			}
		
		}
			break;
		case enZako2Motion::Z2_M_Move:
		{
			//移動を加算する
			m_vPosition.x += (m_TargetPos.x - m_vPosition.x) * MoveSpeed;
			m_vPosition.z += (m_TargetPos.z - m_vPosition.z) * MoveSpeed;

			m_AnimTime += m_AnimSpeed;

			if (m_AnimTime > m_pMesh->GetAnimPeriod(enZako2Animation::Z2_A_StreetSride) - 0.01)
			{
				m_AnimSpeed = 0;
			}


			if (D3DXVec3Length(&m_TargetLength) <= 2.0f)
			{
				//目標の位置に到着したら止まる
				ChangeMotion();
			}

		}
			break;

		case enZako2Motion::Z2_M_Attack:
		{
			m_AnimTime += m_AnimSpeed;

			//攻撃を一回したら待機モーションへ移る
			if (m_AnimTime > m_pMesh->GetAnimPeriod(enZako2Animation::Z2_A_ArmAttack))
			{
				m_IsAttack = false;
				ChangeMotion();
			}
			else if (m_AnimTime > IsAttackHitEnd)
			{
				CSoundManager::Stop(CSoundManager::SE_Z2_Attack);

				//攻撃判定が終わる
				m_IsAttack = false;
			}
			else if (m_AnimTime > IsAttackHitBegin)
			{
				CSoundManager::PlayLoop(CSoundManager::SE_Z2_Attack);

				//攻撃判定が始まる
				m_IsAttack = true;
			}

		}
			break;
		case enZako2Motion::Z2_M_RightSride:
		{
			m_vPosition += vecAxisX * SrideSpeed;

			m_SrideTime++;
			if (m_SrideTime >= MaxSrideTime)
			{
				m_SrideTime = 0;
				ChangeMotion();
			}
		}
			break;
		case enZako2Motion::Z2_M_LeftSride:
		{
			m_vPosition -= vecAxisX * SrideSpeed;
			
			m_SrideTime++;
			if (m_SrideTime >= MaxSrideTime)
			{
				m_SrideTime = 0;
				ChangeMotion();
			}

		}
			break;

		case enZako2Motion::Z2_M_Die:
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
		default:
			break;
		}


		//当たり判定を設定
		m_Capsule.Top		= m_vPosition + D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_Capsule.Bottom = m_vPosition + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Capsule.Radius = 1.0f;

		//レイの位置をプレイヤーの座標にそろえる.
		m_pRayY->Position = m_vPosition;
		//地面にめり込み回避のため、プレイヤーの位置よりも少し上にしておく.
		m_pRayY->Position.y += 0.5f;
		m_pRayY->RotationY = m_vRotation.y;

		//十字(前後左右に伸ばした)レイの設定.
		for (int dir = 0; dir < CROSSRAY::max; dir++)
		{
			m_pCrossRay->Ray[dir].Position = m_vPosition;
			m_pCrossRay->Ray[dir].Position.y += 0.4f;
			m_pCrossRay->Ray[dir].RotationY = m_vRotation.y;
		}



	}
		break;
	case GameOver:
	{
		if (m_AnimNo != enZako2Animation::Z2_A_Move)
		{
			m_AnimNo = enZako2Animation::Z2_A_Move;
			m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
		}

		m_AnimSpeed = 0.03;

		//目標へ向く
		RotationToTarget(CKaito::GetInstance()->GetPosition(), D3DXToRadian(180));

		D3DXVECTOR3 vecAxisX(1.0f, 0.0f, 0.0f);

		//y方向の回転行列.
		D3DXMATRIX mRotationY;
		//Y軸回転行列を作成.
		D3DXMatrixRotationY(
			&mRotationY,	//(out)行列.
			m_vRotation.y);	//プレイヤーのY方向の回転値.

		//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
		D3DXVec3TransformCoord(
			&vecAxisX,		//(out)Z軸ベクトル.
			&vecAxisX,		//(in)Z軸ベクトル.
			&mRotationY);	//Y軸回転行列.


		m_vPosition += vecAxisX * 0.02f;

	}
		break;
	case GameClear:
	{
		if (m_AnimNo != enZako2Animation::Z2_A_FallDown)
		{
			m_AnimNo = enZako2Animation::Z2_A_FallDown;
			m_pMesh->ChangeAnimSet_StartPos(m_AnimNo, m_pMesh->GetAnimPeriod(enZako2Animation::Z2_A_FallDown) - 0.3, m_pAnimCtrl);
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
	default:
		break;
	}








}

void CZako2::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if (m_IsDie == false)
	{
		m_pMesh->SetAnimSpeed(m_AnimSpeed);
		CEnemy::Draw(View, Proj, Light, Camera);
	}
}

void CZako2::RayUpdate()
{
	CEnemy::RayUpdate();
}

void CZako2::DrawShadowMap()
{
	if (m_IsDie == false)
	{
		CEnemy::DrawShadowMap();
	}
}

void CZako2::ChangeMotion(int Motion, int Animation, double ASpeed)
{
	m_AnimTime = 0;
	m_AnimSpeed = ASpeed;
	m_Motion = Motion;
	m_AnimNo = Animation;
	m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
}

void CZako2::ChoiceRandomMotion()
{

	//攻撃、左、右の方向を入れる
	std::vector<std::string> choices
		= { "攻撃", "左", "右"};

	//各選択肢の確立を設定
	std::vector<double> weights = { 0.4, 0.3, 0.3 };

	//ランダムの準備
	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> dist(weights.begin(), weights.end());

	//ランダム選択
	int index = dist(gen);

	CSoundManager::PlaySE(CSoundManager::SE_B_Sride);

	//選択されたもの別に処理をする
	if (choices[index] == "攻撃")
	{
		//攻撃モーションに移る
		ChangeMotion(enZako2Motion::Z2_M_Attack, enZako2Animation::Z2_A_ArmAttack, AnimSpeed);
	}
	else if (choices[index] == "左")
	{
		CSoundManager::PlaySE(CSoundManager::SE_Z2_Sride);

		//右スライド
		ChangeMotion(enZako2Motion::Z2_M_RightSride, enZako2Animation::Z2_A_RightSride, AnimSpeed);
	}
	else if (choices[index] == "右")
	{
		CSoundManager::PlaySE(CSoundManager::SE_Z2_Sride);

		//左スライド
		ChangeMotion(enZako2Motion::Z2_M_LeftSride, enZako2Animation::Z2_A_LeftSride, AnimSpeed);
	}



}
