#pragma once

#include "CStaticMeshObject.h"
#include "CSkinMeshObject.h"
#include "CZako.h"
#include "CZako2.h"
#include "CBoss.h"

/**************************************************
*	地面クラス.
**/
class CGround
	: public CStaticMeshObject
	, public SingletonManager<CGround>
{
public:
	friend class SingletonManager<CGround>;

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;


	//プレイヤーを設定.
	void SetSkinPlayer(CSkinMeshObject& pPlayer) { m_pSkinPlayer = &pPlayer; }
	void SetBoss(CSkinMeshObject& pBoss) { m_pBoss = &pBoss; }

	void SetZako(std::vector<CZako*> pZako) { m_Zako = pZako; }

	void SetZako2(std::vector<CZako2*> pZako2) { m_Zako2 = pZako2; }

protected:
	CGround();
	virtual ~CGround();

	CSkinMeshObject*	m_pSkinPlayer;
	CSkinMeshObject*	m_pBoss;

	std::vector<CZako*> m_Zako;
	std::vector<CZako2*> m_Zako2;
};

