#include "CPhaseBase.h"

CPhaseBase::CPhaseBase()
{
}

CPhaseBase::~CPhaseBase()
{

}

void CPhaseBase::CreateZako(float x, float y, float z)
{

	m_Zako.push_back(new CZako());

	m_Zako[m_Zako.size() - 1]->SetPosition(D3DXVECTOR3(x, y, z));


	m_Zako[m_Zako.size() - 1]->AttachMesh(*CSkinManager::GetMesh(CSkinManager::Zako));

	m_Zako[m_Zako.size() - 1]->SetScene(enScene::GameMain);

	//HPゲージ//
	m_ZakoRedGauge.push_back(new CZakoRedGauge());

	m_ZakoRedGauge[m_ZakoRedGauge.size() - 1]->AttachSprite(*CSprite3DManager::GetSprite(CSprite3DManager::enList::Hp));

	m_ZakoBackGauge.push_back(new CZakoBackGauge());

	m_ZakoBackGauge[m_ZakoBackGauge.size() - 1]->AttachSprite(*CSprite3DManager::GetSprite(CSprite3DManager::enList::Hp));
	////////////////////////////////////////////////

	CGround::GetInstance()->SetZako(m_Zako);

	m_IsZakoDie.push_back(false);



}

void CPhaseBase::CreateZako2(float x, float y, float z)
{
	m_Zako2.push_back(new CZako2());

	m_Zako2[m_Zako2.size() - 1]->AttachMesh(*CSkinManager::GetMesh(CSkinManager::Zako2));
	m_Zako2[m_Zako2.size() - 1]->SetPosition(D3DXVECTOR3(x, y, z));

	m_Zako2[m_Zako2.size() - 1]->SetScene(enScene::GameMain);

	//当たり判定の配置//
	m_Zako2_A_HitPoint.push_back(new CZako2AttackHitPoint());

	m_Zako2_A_HitPoint[m_Zako2_A_HitPoint.size() - 1]->AttachMesh(*CStaticMeshManager::GetMesh(CStaticMeshManager::enList::HitPoint));
	m_Zako2_A_HitPoint[m_Zako2_A_HitPoint.size() - 1]->SetPosition(D3DXVECTOR3(x, y, z));
	//////////////////////////////////////////////////

	//HPゲージ//
	m_Zako2RedGauge.push_back(new CZako2RedGauge());

	m_Zako2RedGauge[m_Zako2RedGauge.size() - 1]->AttachSprite(*CSprite3DManager::GetSprite(CSprite3DManager::enList::Hp));

	m_Zako2BackGauge.push_back(new CZako2BackGauge());

	m_Zako2BackGauge[m_Zako2BackGauge.size() - 1]->AttachSprite(*CSprite3DManager::GetSprite(CSprite3DManager::enList::Hp));
	////////////////////////////////////////////////

	m_IsZako2Die.push_back(false);

	CGround::GetInstance()->SetZako2(m_Zako2);

}

void CPhaseBase::CreateZakoBullet(int no)
{
	m_ZakoBullet.push_back(new CZakoBullet());

	m_ZakoBullet[m_ZakoBullet.size() - 1]->SetRotation(m_Zako[no]->GetRotation());
	m_ZakoBullet[m_ZakoBullet.size() - 1]->SetPosition(m_Zako[no]->GetPosition() + D3DXVECTOR3(0.0f, CZako::EyePos, 0.0f));
	m_ZakoBullet[m_ZakoBullet.size() - 1]->AttachMesh(*CStaticMeshManager::GetMesh(CStaticMeshManager::enList::ZakoBullet));

	m_Zako[no]->SetIsShot(false);

	CSoundManager::PlaySE(CSoundManager::SE_Z_Shot);

}

void CPhaseBase::ZakoSetUpdate()
{

	for (auto& zb : m_ZakoBullet)
	{
		zb->Update();

	}

	for (size_t no = 0; no < m_Zako.size(); no++)
	{
		m_Zako[no]->Update();

		if (m_Zako[no]->GetIsShot() == true)
		{
			//弾の発射
			CreateZakoBullet(no);
		}

		if (CCapsuleCollision::IsHit(CKaito::GetInstance()->GetCapsule(), m_Zako[no]->GetCapsule())) {
			if (m_Zako[no]->GetIsTackleMove() == true)
			{
				//突進攻撃時の当たり判定処理
				m_Zako[no]->GetCapsule().HitJudg = false;
				if (CKaito::GetInstance()->GetMotion() != CKaito::P_M_Die)
				{
					CSoundManager::PlaySE(CSoundManager::SE_P_Damage);
					CKaito::GetInstance()->Damage(10.0f);
				}
			}
		}
		for (int a = 0; a <= CKaito::GetInstance()->P_C_Attack3; a++) {
			if (CCapsuleCollision::IsHit(CKaito::GetInstance()->GetAttackColl(a), m_Zako[no]->GetCapsule()))
			{
				//カイト攻撃時の当たり判定処理
				CKaito::GetInstance()->GetAttackColl(a).SetHitJudg(false);
				m_Zako[no]->Damage(CKaito::SwordDamage);
			}
		}

		////HPゲージの位置を設定////
		m_ZakoRedGauge[no]->SetZakoHp(m_Zako[no]->GetHp());
		m_ZakoBackGauge[no]->SetZakoHp(m_Zako[no]->GetHp());


		m_ZakoBackGauge[no]->SetPosition(m_Zako[no]->GetPosition() + D3DXVECTOR3(0.0f, 2.0f, 0.0f));
		m_ZakoRedGauge[no]->SetPosition(m_Zako[no]->GetPosition() + D3DXVECTOR3(0.0f, 2.0f, 0.0f));
		///////////////////////////

		//すべてのザコが生きているかの記憶をする
		m_IsZakoDie[no] = m_Zako[no]->GetIsDie();

		for (size_t ano = 0; ano < m_Zako.size(); ano++)
		{
			if (no != ano)
			{
				//押し戻し処理
				if (CCapsuleCollision::PushBack(m_Zako[no]->GetCapsule(), m_Zako[ano]->GetCapsule()))
				{
					m_Zako[no]->SetPosition(m_Zako[no]->GetCapsule().Bottom.x, m_Zako[no]->GetPosition().y, m_Zako[no]->GetCapsule().Bottom.z);
					m_Zako[ano]->SetPosition(m_Zako[ano]->GetCapsule().Bottom.x, m_Zako[ano]->GetPosition().y, m_Zako[ano]->GetCapsule().Bottom.z);
				}

			}
		}

	}

	for (size_t zb = 0; zb < m_ZakoBullet.size(); zb++)
	{
		if (CCapsuleCollision::IsHit(CKaito::GetInstance()->GetCapsule(), m_ZakoBullet[zb]->GetCapsule()))
		{
			//ザコ弾の当たり判定
			m_ZakoBullet[zb]->SetShotting(false);
			if (CKaito::GetInstance()->GetMotion() != CKaito::P_M_Die)
			{
				CSoundManager::PlaySE(CSoundManager::SE_P_Damage);

				CKaito::GetInstance()->Damage(CZakoBullet::BulletDamage);
			}
		}
	}

	for (auto& z : m_ZakoBackGauge)
	{
		z->Update();
	}
	for (auto& z : m_ZakoRedGauge)
	{
		z->Update();
	}
}

void CPhaseBase::Zako2SetUpdate()
{
	for (auto& z2_AH : m_Zako2_A_HitPoint)
	{
		z2_AH->Update();
	}

	for (size_t no = 0; no < m_Zako2.size(); no++)
	{
		m_Zako2[no]->Update();

		m_Zako2_A_HitPoint[no]->SetIsHitAttack(m_Zako2[no]->GetIsAttack());

		if (m_Zako2[no]->GetIsAttack() == true)
		{
			//アタック判定の位置をセットする
			m_Zako2_A_HitPoint[no]->SetTargetPos(m_Zako2[no]->GetPosition());
			m_Zako2_A_HitPoint[no]->SetTargetRot(m_Zako2[no]->GetRotation().y);
		}

		for (int a = 0; a <= CKaito::GetInstance()->P_C_Attack3; a++) {
			if (CCapsuleCollision::IsHit(CKaito::GetInstance()->GetAttackColl(a), m_Zako2[no]->GetCapsule()))
			{
				//カイト攻撃時の当たり判定処理
				CKaito::GetInstance()->GetAttackColl(a).SetHitJudg(false);
				m_Zako2[no]->Damage(CKaito::SwordDamage);
			}
		}

		////HPゲージの位置を設定////
		m_Zako2RedGauge[no]->SetZako2Hp(m_Zako2[no]->GetHp());
		m_Zako2BackGauge[no]->SetZako2Hp(m_Zako2[no]->GetHp());


		m_Zako2BackGauge[no]->SetPosition(m_Zako2[no]->GetPosition() + D3DXVECTOR3(0.0f, 2.0f, 0.0f));
		m_Zako2RedGauge[no]->SetPosition(m_Zako2[no]->GetPosition() + D3DXVECTOR3(0.0f, 2.0f, 0.0f));
		///////////////////////////

		//すべてのザコ2が生きているかの記憶をする
		m_IsZako2Die[no] = m_Zako2[no]->GetIsDie();

		for (size_t ano = 0; ano < m_Zako2.size(); ano++)
		{
			if (no != ano)
			{
				//押し戻し処理
				if (CCapsuleCollision::PushBack(m_Zako2[no]->GetCapsule(), m_Zako2[ano]->GetCapsule()))
				{
					m_Zako2[no]->SetPosition(m_Zako2[no]->GetCapsule().Bottom.x, m_Zako2[no]->GetPosition().y, m_Zako2[no]->GetCapsule().Bottom.z);
					m_Zako2[ano]->SetPosition(m_Zako2[ano]->GetCapsule().Bottom.x, m_Zako2[ano]->GetPosition().y, m_Zako2[ano]->GetCapsule().Bottom.z);
				}

			}
		}
	}

	for (size_t z2_AH = 0; z2_AH < m_Zako2_A_HitPoint.size(); z2_AH++)
	{
		if (m_Zako2_A_HitPoint[z2_AH]->GetIsHitAttack() == true)
		{
			if (CCapsuleCollision::IsHit(CKaito::GetInstance()->GetCapsule(), m_Zako2_A_HitPoint[z2_AH]->GetCapsule()))
			{
				//ザコ２の攻撃時の当たり判定処理
				if (CKaito::GetInstance()->GetMotion() != CKaito::P_M_Die)
				{
					CSoundManager::PlaySE(CSoundManager::SE_P_Damage);

					CKaito::GetInstance()->Damage(0.2f);
				}
			}
		}
	}


	for (auto& z2 : m_Zako2BackGauge)
	{
		z2->Update();
	}
	for (auto& z2 : m_Zako2RedGauge)
	{
		z2->Update();
	}

}

void CPhaseBase::ZakoSetDraw(D3DXMATRIX& View, D3DXMATRIX& Proj,
	LIGHT& Light, CAMERA& Camera)
{
	for (auto& z : m_Zako)
	{
		z->Draw(View, Proj, Light, Camera);
	}

	for (auto& zb : m_ZakoBullet)
	{
		zb->Draw(View, Proj, Light, Camera);
	}

	CDirectX11::GetInstance()->SetDepth(false);
	for (auto& z : m_ZakoBackGauge)
	{
		z->Draw(View, Proj);
	}
	for (auto& z : m_ZakoRedGauge)
	{
		z->Draw(View, Proj);
	}
	CDirectX11::GetInstance()->SetDepth(true);
}

void CPhaseBase::Zako2SetDraw(D3DXMATRIX& View, D3DXMATRIX& Proj,
	LIGHT& Light, CAMERA& Camera)
{
	for (auto& z : m_Zako2)
	{
		z->Draw(View, Proj, Light, Camera);
	}

	CDirectX11::GetInstance()->SetDepth(false);
	for (auto& z2 : m_Zako2BackGauge)
	{
		z2->Draw(View, Proj);
	}
	for (auto& z2 : m_Zako2RedGauge)
	{
		z2->Draw(View, Proj);
	}

	CDirectX11::GetInstance()->SetDepth(true);

}

bool CPhaseBase::LoadEnemyPos(const std::string& filename)
{
	std::ifstream file(filename);
	std::string line;

	if (!file.is_open())
	{
		std::cerr << "ファイルを開けませんでした: " << filename << std::endl;
		return false;
	}

	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		std::string id, xStr, zStr;

		if (std::getline(ss, id, ',')
			&& std::getline(ss, xStr, ',')
			&& std::getline(ss, zStr, ','))
		{
			//ザコの場合
			if (id == "Zako")
			{
				CreateZako(std::stof(xStr), 0.0f, std::stof(zStr));
			}
			//ザコ２の場合
			else if (id == "Zako2")
			{
				CreateZako2(std::stof(xStr), 0.0f, std::stof(zStr));
			}
		}

	}

	return true;
}
