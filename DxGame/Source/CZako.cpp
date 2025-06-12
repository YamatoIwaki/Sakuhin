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


		//プレイヤーとの一定距離の場所を設定する
		TargetPosSet(RadiusOfTarget);

		if (m_Motion != enZakoMotion::Z_M_Tackle && m_Motion != enZakoMotion::Z_M_Die)
		{
			RotationToTarget(CKaito::GetInstance()->GetPosition());
		}
		//目標との距離
		m_TargetLength = m_TargetPos - m_vPosition;


		switch (m_Motion)
		{
			//待機中
		case CZako::Z_M_Stop:
		{
			m_AttackMotionTime++;

			m_IsShot = false;

			//離れすぎたら移動モーションに移る
			if (D3DXVec3Length(&m_TargetLength) > OverRadiusOfTarget)
			{
				ChangeMotion(enZakoMotion::Z_M_Move, enZakoAnimation::Z_A_Move);

			}
			else if (m_AttackMotionTime > MaxAttackCoolTime) {
				//時間をゼロにする
				m_AttackMotionTime = 0;
				//モーションを選ぶ
				ChoiceRandomMotion();
			}

		}
		break;
		//移動中
		case CZako::Z_M_Move:
		{
			//移動を加算する
			m_vPosition.x += (m_TargetPos.x - m_vPosition.x) * MoveSpeed;
			m_vPosition.z += (m_TargetPos.z - m_vPosition.z) * MoveSpeed;


			if (D3DXVec3Length(&m_TargetLength) <= 2.0f)
			{
				//目標の位置に到着したら止まる
				ChangeMotion();
			}

		}
		break;
		//発射準備中
		case CZako::Z_M_Shot:
		{
			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 0.001)
			{
				m_AnimTime = 0;
				m_IsShot = true;

				//目標が達成したら止まる
				ChangeMotion();
			}


		}
		break;
		//突進攻撃
		case CZako::Z_M_Tackle:
		{
			if (m_AnimSpeed > TackleReadySpeed)
			{
				CSoundManager::PlayLoop(CSoundManager::SE_Z_Tacle);

				m_TackleTime++;
				m_IsTackleMove = true;
				//移動を加算する
				m_vPosition.x += (m_TacklePos.x - m_vPosition.x) * TackleSpeed;
				m_vPosition.z += (m_TacklePos.z - m_vPosition.z) * TackleSpeed;

				m_TackleLength = m_TacklePos - m_vPosition;

				if (D3DXVec3Length(&m_TackleLength) <= 1.0f || m_TackleTime > MaxTackleTime)
				{
					CSoundManager::Stop(CSoundManager::SE_Z_Tacle);

					m_TackleTime = 0;
					m_IsTackleMove = false;
					m_Capsule.HitJudg = true;
					//目標の位置に到着したら止まる
					ChangeMotion();
				}
			}
			else
			{
				m_TackleTime = 0;
				//アニメーションスピードを加算
				m_AnimSpeed += TackleAnimSumSpeed;
			}


		}
		break;
		//死ンでいる
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


		//当たり判定を設定
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

		//目標へ向く
		RotationToTarget(CKaito::GetInstance()->GetPosition(), D3DXToRadian(0));

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
	//左、右、後ろの方向を入れる
	std::vector<std::string> choices
		= { "ショット", "タックル" };

	//各選択肢の確立を設定
	std::vector<double> weights = { 0.7, 0.3 };

	//ランダムの準備
	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> dist(weights.begin(), weights.end());

	//ランダム選択
	int index = dist(gen);

	CSoundManager::PlaySE(CSoundManager::SE_B_Sride);

	//選択されたもの別に処理をする
	if (choices[index] == "ショット")
	{
		//ショットモーション
		ChangeMotion(enZakoMotion::Z_M_Shot, Z_A_Appear, AnimApperSpeed);
	}
	else if (choices[index] == "タックル")
	{
		//突進モーション
		ChangeMotion(enZakoMotion::Z_M_Tackle, Z_A_Move, AnimMoveSpeed);
		TacklePosSet();
	}

}

