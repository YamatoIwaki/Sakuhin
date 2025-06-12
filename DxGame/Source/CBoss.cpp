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
		//Hpが０になると死ぬモーションに入る
		if (m_Hp <= 0.0f && m_Motion != enBossMotion::B_M_Die)
		{
			CSoundManager::PlaySE(CSoundManager::SE_B_Die);
			ChangeMotion(B_M_Die, B_A_BeginLose);
		}

		//プレイヤーとの一定距離の場所を設定する
		if (m_Motion != B_M_SwordAttack && m_Motion != B_M_Die && m_Motion != B_M_Boot)
		{
			TargetPosSet(RadiusOfTargetDefalt);
			RotationToTarget(CKaito::GetInstance()->GetPosition());
		}
		
		//目標との距離
		m_TargetLength = m_TargetPos - m_vPosition;

		D3DXVECTOR3 vecAxisX(1.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);

		//y方向の回転行列.
		D3DXMATRIX mRotationY;
		//Y軸回転行列を作成.
		D3DXMatrixRotationY(
			&mRotationY,	//(out)行列.
			m_vRotation.y);	//プレイヤーのY方向の回転値.

		//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
		D3DXVec3TransformCoord(
			&vecAxisX,		//(out)X軸ベクトル.
			&vecAxisX,		//(in)X軸ベクトル.
			&mRotationY);	//Y軸回転行列.

		//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
		D3DXVec3TransformCoord(
			&vecAxisZ,		//(out)Z軸ベクトル.
			&vecAxisZ,		//(in)Z軸ベクトル.
			&mRotationY);	//Y軸回転行列.


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

			//離れすぎたら移動モーションに移る
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
			//移動を加算する
			m_vPosition.x += (m_TargetPos.x - m_vPosition.x) * MoveSpeed;
			m_vPosition.z += (m_TargetPos.z - m_vPosition.z) * MoveSpeed;
			
			if (m_AnimNo == enBossAnimation::B_A_BeginStraight)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 0.01)
				{
					//発信初めが終わったら移動中アニメーションにする
					m_AnimNo = enBossAnimation::B_A_Straight;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (D3DXVec3Length(&m_TargetLength) <= 2.0f)
			{
				if (m_AnimNo == enBossAnimation::B_A_Straight)
				{
					//目標の位置に到着したら止まるアニメーションにする
					m_AnimNo = enBossAnimation::B_A_EndStraight;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_AnimNo == enBossAnimation::B_A_EndStraight)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime)
				{
					//止まるアニメーションが終わったら待機状態にする
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
					//発信初めが終わったら移動中アニメーションにする
					m_AnimNo = enBossAnimation::B_A_RightStep;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_StepTime >= MaxStepTime)
			{
				if (m_AnimNo == enBossAnimation::B_A_RightStep)
				{
					//目標の位置に到着したら止まるアニメーションにする
					m_AnimNo = enBossAnimation::B_A_EndRightStep;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_AnimNo == enBossAnimation::B_A_EndRightStep)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime)
				{
					//止まるアニメーションが終わったら待機状態にする
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
					//発信初めが終わったら移動中アニメーションにする
					m_AnimNo = enBossAnimation::B_A_LeftStep;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_StepTime >= MaxStepTime)
			{
				if (m_AnimNo == enBossAnimation::B_A_LeftStep)
				{
					//目標の位置に到着したら止まるアニメーションにする
					m_AnimNo = enBossAnimation::B_A_EndLeftStep;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_AnimNo == enBossAnimation::B_A_EndLeftStep)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime)
				{
					//止まるアニメーションが終わったら待機状態にする
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
					//発信初めが終わったら移動中アニメーションにする
					m_AnimNo = enBossAnimation::B_A_BackStep;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_StepTime >= MaxStepTime)
			{
				if (m_AnimNo == enBossAnimation::B_A_BackStep)
				{
					//目標の位置に到着したら止まるアニメーションにする
					m_AnimNo = enBossAnimation::B_A_EndBackStep;
					m_pMesh->ChangeAnimSet(m_AnimNo, m_pAnimCtrl);
				}
			}

			if (m_AnimNo == enBossAnimation::B_A_EndBackStep)
			{
				m_AnimTime += m_AnimSpeed;
				if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime)
				{
					//止まるアニメーションが終わったらミサイル攻撃をする
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
				//剣の振り始め(一度だけ切り替える)
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
				//攻撃を一度したら待機状態に戻る
				ChangeMotion();
			}

			//近くなるまでを加算する
			m_vPosition.x += (m_TargetPos.x - m_vPosition.x) * SordAttackMoveSpeed;
			m_vPosition.z += (m_TargetPos.z - m_vPosition.z) * SordAttackMoveSpeed;

		}
			break;
		case CBoss::B_M_Missile:
		{
			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(m_AnimNo) <= m_AnimTime + 1.0)
			{
				//セットが完了したら発射の合図を送る
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
				//アニメーションを最後までしたら待機状態に戻る
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

		//当たり判定を設定
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
	//左、右、後ろの方向を入れる
	std::vector<std::string> choices 
		= { "右", "左", "後ろ"};

	//各選択肢の確立を設定
	std::vector<double> weights = { 0.4, 0.4, 0.2 };

	//ランダムの準備
	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> dist(weights.begin(), weights.end());

	//ランダム選択
	int index = dist(gen);

	CSoundManager::PlaySE(CSoundManager::SE_B_Sride);

	//選択されたもの別に処理をする
	if (choices[index] == "右")
	{
		ChangeMotion(enBossMotion::B_M_RightStep, enBossAnimation::B_A_BeginRightStep);
	}
	else if (choices[index] == "左")
	{
		ChangeMotion(enBossMotion::B_M_LeftStep, enBossAnimation::B_A_BeginLeftStep);
	}
	else if (choices[index] == "後ろ")
	{
		ChangeMotion(enBossMotion::B_M_BackStep, enBossAnimation::B_A_BeginBackStep);
	}

}

void CBoss::WhatAttack()
{
	//剣とミサイルを入れる
	std::vector<std::string> choices
		= { "剣", "ミサイル" };

	//選択肢の確率を距離によって変わるよう設定
#if true

	D3DXVECTOR3 length = CKaito::GetInstance()->GetPosition() - m_vPosition;

	std::vector<double> weights = CulculateWeightsFromDistance(D3DXVec3Length(&length));
#else
	std::vector<double> weights = { 1.0, 0.0};
#endif

	//ランダムの準備
	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> dist(weights.begin(), weights.end());

	//ランダム選択
	int index = dist(gen);

	if (choices[index] == "剣")
	{
		//剣攻撃に入る際近づくよう目標位置をセットする

		TargetPosSet(RadiusOfSwordAttack);
		ChangeMotion(enBossMotion::B_M_SwordAttack, enBossAnimation::B_A_SwordAttack);
	}
	else if (choices[index] == "ミサイル")
	{
		ChangeMotion(enBossMotion::B_M_Missile, enBossAnimation::B_A_Missile);
	}

}

std::vector<double> CBoss::CulculateWeightsFromDistance(float distance)
{
	//距離が0~20で変化する
	float nearWeight	= 1.0f / (distance + 0.1f);		//近距離攻撃(最小値が0.0fにならないように+1.0fをする)

	float farWeight	= distance / MaxAttackLength;				//遠距離攻撃

	//double型に変換して返す
	return {static_cast<double>(nearWeight),
			static_cast<double>(farWeight)};

}
