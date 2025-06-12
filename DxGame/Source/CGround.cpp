#include "CGround.h"

CGround::CGround()
	: m_pSkinPlayer	(nullptr)
	, m_pBoss		(nullptr)
	, m_Zako		()
	, m_Zako2		()
{
	m_vScale *= 10.0f;
}

CGround::~CGround()
{
	m_pSkinPlayer	= nullptr;
	m_pBoss			= nullptr;
}

void CGround::Init()
{
	m_Scene = enScene::GameMain;
}

void CGround::Update()
{
	//----------------------------------------
	// Y軸のレイ処理
	//----------------------------------------
		RAY Kray = m_pSkinPlayer->GetRayY();
		float KDistance = 0.0f;
		D3DXVECTOR3 KIntersect(0.0f, 0.0f, 0.0f);

		//スキンメッシュのプレイヤーとレイと当たり判定.
		if (IsHitForRay(Kray, &KDistance, &KIntersect) == true)
		{
			D3DXVECTOR3 Pos = m_pSkinPlayer->GetPosition();
			Pos.y = KIntersect.y + 0.02f;	//1.0fで少し上へ補正.
			m_pSkinPlayer->SetPosition(Pos);
		}
		
		RAY Bray = m_pBoss->GetRayY();
		float BDistance = 0.0f;
		D3DXVECTOR3 BIntersect(0.0f, 0.0f, 0.0f);

		//ボスとレイと当たり判定.
		if (IsHitForRay(Bray, &BDistance, &BIntersect) == true)
		{
			D3DXVECTOR3 Pos = m_pBoss->GetPosition();
			Pos.y = BIntersect.y + 0.02f;	//1.0fで少し上へ補正.
			m_pBoss->SetPosition(Pos);
		}

		for (auto z : m_Zako)
		{
			RAY Zray = z->GetRayY();
			float Distance = 0.0f;
			D3DXVECTOR3 Intersect(0.0f, 0.0f, 0.0f);

			//ザコとレイと当たり判定.
			if (IsHitForRay(Zray, &Distance, &Intersect) == true)
			{
				D3DXVECTOR3 Pos = z->GetPosition();
				Pos.y = Intersect.y + 0.02f;	//1.0fで少し上へ補正.
				z->SetPosition(Pos);
			}
		}

		for (auto z2 : m_Zako2)
		{
			RAY Z2ray = z2->GetRayY();
			float Distance = 0.0f;
			D3DXVECTOR3 Intersect(0.0f, 0.0f, 0.0f);

			//ザコ2とレイと当たり判定.
			if (IsHitForRay(Z2ray, &Distance, &Intersect) == true)
			{
				D3DXVECTOR3 Pos = z2->GetPosition();
				Pos.y = Intersect.y + 0.02f;	//1.0fで少し上へ補正.
				z2->SetPosition(Pos);
			}
		}


		//----------------------------------------
		// 十字のレイ処理
		//----------------------------------------
		CROSSRAY sCrossRay = m_pSkinPlayer->GetCrossRay();

		//壁との当たり判定.
		CalculatePositionFromWall(&sCrossRay);
		D3DXVECTOR3 sPos = m_pSkinPlayer->GetPosition();
		sPos.x = sCrossRay.Ray[CROSSRAY::XL].Position.x;
		sPos.z = sCrossRay.Ray[CROSSRAY::XL].Position.z;
		m_pSkinPlayer->SetPosition(sPos);

		CROSSRAY bCrossRay = m_pBoss->GetCrossRay();

		//壁との当たり判定.
		CalculatePositionFromWall(&bCrossRay);
		D3DXVECTOR3 bPos = m_pBoss->GetPosition();
		bPos.x = bCrossRay.Ray[CROSSRAY::XL].Position.x;
		bPos.z = bCrossRay.Ray[CROSSRAY::XL].Position.z;
		m_pBoss->SetPosition(bPos);

		for (auto z : m_Zako)
		{
			CROSSRAY zCrossRay = z->GetCrossRay();

			//壁との当たり判定.
			CalculatePositionFromWall(&zCrossRay);
			D3DXVECTOR3 zPos = z->GetPosition();
			zPos.x = zCrossRay.Ray[CROSSRAY::XL].Position.x;
			zPos.z = zCrossRay.Ray[CROSSRAY::XL].Position.z;
			z->SetPosition(zPos);
		}

		for (auto z2 : m_Zako2)
		{
			CROSSRAY z2CrossRay = z2->GetCrossRay();

			//壁との当たり判定.
			CalculatePositionFromWall(&z2CrossRay);
			D3DXVECTOR3 z2Pos = z2->GetPosition();
			z2Pos.x = z2CrossRay.Ray[CROSSRAY::XL].Position.x;
			z2Pos.z = z2CrossRay.Ray[CROSSRAY::XL].Position.z;
			z2->SetPosition(z2Pos);
		}


}

void CGround::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}
