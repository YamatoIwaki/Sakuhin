#pragma once
#include "CKaito.h"
#include "CBoss.h"
#include "CBossMissile.h"

#include "CStaticMesh.h"

#include "CStaticMeshManager.h"

/***********************************************
*			�{�X�~�T�C���̓���
**/



class CBossMissileSet
	: public SingletonManager<CBossMissileSet>
{
public:
	static constexpr int MissileMax = 4;	//�~�T�C���̍ő�e��

private:
	CBossMissileSet();
	~CBossMissileSet();
public:

	friend class SingletonManager<CBossMissileSet>;

	void Update();

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) ;


	std::vector<CBossMissile*> GetMissile() { return m_BossMissile; }


private:

	void SetMissile();



private:
	std::vector<CBossMissile*>	m_BossMissile;


};

