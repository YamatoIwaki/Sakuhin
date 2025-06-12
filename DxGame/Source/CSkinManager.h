#pragma once
#include "CSkinMesh.h"
#include "CSingletonManager.h"

/*********************************************************
*			スタティックメッシュマネージャー
**/




class CSkinManager
	: public SingletonManager<CSkinManager>
{
public:
	enum enList
	{
		Kaito,		//カイト
		Zako,		//ザコ
		Zako2,		//ザコ２
		Boss,		//ボス


		//追加したら上に追加
		max,
	};


private:
	CSkinManager();
public:
	friend class SingletonManager<CSkinManager>;

	~CSkinManager();

	//データ読込関数.
	bool Load();


	static CSkinMesh* GetMesh(enList list) { return CSkinManager::GetInstance()->m_pSkinMesh[list]; }


private:

	CSkinMesh* m_pSkinMesh[enList::max];

};

