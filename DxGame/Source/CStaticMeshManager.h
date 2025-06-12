#pragma once
#include "CStaticMesh.h"
#include "CSingletonManager.h"

/*********************************************************
*			スタティックメッシュマネージャー
**/




class CStaticMeshManager
	: public SingletonManager<CStaticMeshManager>
{
public:
	enum enList
	{
		ZakoBullet,		//ザコの弾
		Missile,		//ミサイル
		Stage,			//ステージ
		HitPoint,		//当たり判定の位置
		RangeOfStage,	//ステージの移動範囲
		ApperRangeOfStage,	//ステージの移動範囲(視覚化)

		//追加したら上に追加
		max,
	};


private:
	CStaticMeshManager();
public:
	friend class SingletonManager<CStaticMeshManager>;

	~CStaticMeshManager();

	//データ読込関数.
	bool Load();


	static CStaticMesh* GetMesh(enList list) { return CStaticMeshManager::GetInstance()->m_pStaticMesh[list]; }


private:

	CStaticMesh* m_pStaticMesh[enList::max];

};

