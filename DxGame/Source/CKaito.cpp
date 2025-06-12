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
				//HPが０以下になったら死ぬ
				CSoundManager::PlaySE(CSoundManager::SE_P_Die);
				ChangeMotion(enPlayerMotion::P_M_Die, enPlayerAnim::P_A_Dead, 0.003);
			}


			if (m_PlayerMotion != enPlayerMotion::P_M_Rolling)
			{
				//クールタイム
				m_RollingCoolTime++;
				if (GetAsyncKeyState(VK_SHIFT) & 0x8000 && m_RollingCoolTime >= RollingResetTime)
				{
					/*****************移動計算********************/
					//Z軸ベクトル(z+方向への単位ベクトル)
					//※長さ(大きさ)　が１のベクトルを単位ベクトルという.
					m_RollingvecZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

					//y方向の回転行列.
					D3DXMATRIX mRotationY;
					//Y軸回転行列を作成.
					D3DXMatrixRotationY(
						&mRotationY,	//(out)行列.
						m_QuaterRot.y);	//プレイヤーのY方向の回転値.

					//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
					D3DXVec3TransformCoord(
						&m_RollingvecZ,		//(out)Z軸ベクトル.
						&m_RollingvecZ,		//(in)Z軸ベクトル.
						&mRotationY);	//Y軸回転行列.

					/********************************************/

					//回転モーションへ移行
					ChangeMotion(enPlayerMotion::P_M_Rolling, enPlayerAnim::P_A_Rolling);
				}
			}
		}
		AttackRange();

		switch (m_PlayerMotion)
		{
		case CKaito::P_M_Stop:			//待機
		{
			if (GetAsyncKeyState('W') || GetAsyncKeyState('S'))
			{
				if (GetAsyncKeyState('W') && GetAsyncKeyState('S'))
				{ /*前と後ろがぶつかっていたら処理をしない*/
				}
				else
				{
					//移動モーションへ
					ChangeMotion(enPlayerMotion::P_M_Move, enPlayerAnim::P_A_Run);
				}
			}
			if (GetAsyncKeyState('A') || GetAsyncKeyState('D'))
			{
				if (GetAsyncKeyState('A') && GetAsyncKeyState('D'))
				{ /*左と右がぶつかっていたら処理をしない*/
				}
				else
				{
					//移動モーションへ
					ChangeMotion(enPlayerMotion::P_M_Move, enPlayerAnim::P_A_Run);
				}
			}


			if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
			{
				CSoundManager::PlaySE(CSoundManager::SE_P_Sword1);
				m_AttackCapsule[P_C_Attack1].HitJudg = true;
				//攻撃モーションへ
				ChangeMotion(enPlayerMotion::P_M_Attack1, enPlayerAnim::P_A_Attack1);
			}



		}
		break;
		case CKaito::P_M_Move:			//移動
		{
			if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
			{
				m_AttackCapsule[P_C_Attack1].HitJudg = true;
				//攻撃モーションへ
				ChangeMotion(enPlayerMotion::P_M_Attack1, enPlayerAnim::P_A_Attack1);
			}

			//前
			if (GetAsyncKeyState('W') & 0x8000) {
				m_MoveAround = enMoveAround::Move_Forward;
			}
			//後ろ.
			if (GetAsyncKeyState('S') & 0x8000) {
				if (GetAsyncKeyState('W') & 0x8000) {
					//まだ押している状態は移動状態にする
					m_MoveAround = enMoveAround::Move_Around_Stop;	//前と後ろが相対していたら止まる
				}
				else {
					m_MoveAround = enMoveAround::Move_Backward;
				}
			}


			//右
			if (GetAsyncKeyState('D') & 0x8000) {
				m_MoveAttendDance = enMoveAttendDance::Move_Rightward;
			}
			//左
			if (GetAsyncKeyState('A') & 0x8000) {
				if (GetAsyncKeyState('D') & 0x8000) {
					m_MoveAttendDance = enMoveAttendDance::Move_Attend_Stop; //右と左が相対していたら止まる
				}
				else {
					m_MoveAttendDance = enMoveAttendDance::Move_Leftward;
				}
			}

			//どれも押されていなければ待機モーションに移る
			if (m_IsAround == false && m_IsAttend == false)
			{
				ChangeMotion();
			}
			//斜め移動になったらコントロール値を入れてあげる
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
		case CKaito::P_M_Rolling:		//回転移動
		{

			m_AnimTime += m_AnimSpeed;

			//回転しながら移動
			m_vPosition += m_RollingvecZ * RollingSpeed;

			//trueになっているときがあるため
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

		case CKaito::P_M_Attack1:		//攻撃一段階目
		{
			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Attack1) - 0.02 <= m_AnimTime)
			{
				m_AttackCapsule[P_C_Attack1].HitJudg = false;
				//何もしなければ待機モーション
				ChangeMotion();
			}
			else if (m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Attack1) - 0.8 <= m_AnimTime)
			{
				if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
				{
					CSoundManager::PlaySE(CSoundManager::SE_P_Sword2);
					m_AttackCapsule[P_C_Attack1].HitJudg = false;
					m_AttackCapsule[P_C_Attack2].HitJudg = true;
					//攻撃第二段階へ移行
					ChangeMotion(enPlayerMotion::P_M_Attack2, enPlayerAnim::P_A_Attack2);

				}
			}

		}
		break;
		case CKaito::P_M_Attack2:			//攻撃二段階目
		{
			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Attack2) - 0.02 <= m_AnimTime)
			{
				m_AttackCapsule[P_C_Attack2].HitJudg = false;
				//何もしなければ待機モーション
				ChangeMotion();
			}
			else if (m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Attack2) - 0.8 <= m_AnimTime)
			{
				if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
				{
					CSoundManager::PlaySE(CSoundManager::SE_P_Sword3);
					m_AttackCapsule[P_C_Attack2].HitJudg = false;
					m_AttackCapsule[P_C_Attack3].HitJudg = true;
					//攻撃第二段階へ移行
					ChangeMotion(enPlayerMotion::P_M_Attack3, enPlayerAnim::P_A_Attack3);

				}
			}


		}
		break;

		case CKaito::P_M_Attack3:			//攻撃三段階目
		{
			m_AnimTime += m_AnimSpeed;

			if (m_pMesh->GetAnimPeriod(enPlayerAnim::P_A_Attack3) - 0.01 <= m_AnimTime)
			{
				m_AttackCapsule[P_C_Attack3].HitJudg = false;
				if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
				{
					m_AttackCapsule[P_C_Attack1].HitJudg = true;
					//攻撃第一段階へ戻る
					ChangeMotion(enPlayerMotion::P_M_Attack1, enPlayerAnim::P_A_Attack1);
				}
				else
				{
					//何もしなければ待機モーション
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

		//当たり判定を設定
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

	//レイの位置をプレイヤーの座標にそろえる.
	m_pRayY->Position = m_vPosition;
	//地面にめり込み回避のため、プレイヤーの位置よりも少し上にしておく.
	m_pRayY->Position.y += 0.3f;
	m_pRayY->RotationY = m_vRotation.y;

	//十字(前後左右に伸ばした)レイの設定.
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

//操作
void CKaito::MoveControl()
{

	/*****************移動計算********************/
	//Z軸ベクトル(z+方向への単位ベクトル)
	//※長さ(大きさ)　が１のベクトルを単位ベクトルという.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 vecAxisX(1.0f, 0.0f, 0.0f);

	D3DXVECTOR3 direction = CCamera::GetInstance()->GetPosition() - m_vPosition;

	m_DefaAngle = atan2f(direction.x, direction.z);

	//y方向の回転行列.
	D3DXMATRIX mRotationY;
	//Y軸回転行列を作成.
	D3DXMatrixRotationY(
		&mRotationY,	//(out)行列.
		m_DefaAngle - D3DXToRadian(180));	//プレイヤーのY方向の回転値.

	//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
	D3DXVec3TransformCoord(
		&vecAxisZ,		//(out)Z軸ベクトル.
		&vecAxisZ,		//(in)Z軸ベクトル.
		&mRotationY);	//Y軸回転行列.

	//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
	D3DXVec3TransformCoord(
		&vecAxisX,		//(out)Z軸ベクトル.
		&vecAxisX,		//(in)Z軸ベクトル.
		&mRotationY);	//Y軸回転行列.
/********************************************/




	//前後移動
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

	//左右移動
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
	//Z軸ベクトル(z+方向への単位ベクトル)
	//※長さ(大きさ)　が１のベクトルを単位ベクトルという.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);

	//y方向の回転行列.
	D3DXMATRIX mRotationY;
	//Y軸回転行列を作成.
	D3DXMatrixRotationY(
		&mRotationY,	//(out)行列.
		m_QuaterRot.y);	//プレイヤーのY方向の回転値.

	//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
	D3DXVec3TransformCoord(
		&vecAxisZ,		//(out)Z軸ベクトル.
		&vecAxisZ,		//(in)Z軸ベクトル.
		&mRotationY);	//Y軸回転行列.


	for (size_t n = 0; n < m_AttackCapsule.size(); n++)
	{
		m_AttackCapsule[n].Top = m_vPosition + (vecAxisZ * 1.5f);
		m_AttackCapsule[n].Bottom = m_vPosition + (vecAxisZ * 1.5f);
	}
}



