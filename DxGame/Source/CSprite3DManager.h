#pragma once
#include "CSprite3D.h"
#include "CSingletonManager.h"

/*********************************************************
*			スプライト3Dマネージャー
**/




class CSprite3DManager
	: public SingletonManager<CSprite3DManager>
{
public:
	enum enList
	{
		Hp,		//Hp

		//追加したら上に追加
		max,
	};


private:
	CSprite3DManager();
public:
	friend class SingletonManager<CSprite3DManager>;

	~CSprite3DManager();

	//データ読込関数.
	bool Load();


	static CSprite3D* GetSprite(enList list) { return CSprite3DManager::GetInstance()->m_pSprite3D[list]; }


private:

	CSprite3D* m_pSprite3D[enList::max];

};

