#include "CSkinManager.h"

CSkinManager::CSkinManager()
	: m_pSkinMesh()
{
	//インスタンス生成.
	for (int i = 0; i < enList::max; i++)
	{
		m_pSkinMesh[i] = new CSkinMesh();
	}
}

CSkinManager::~CSkinManager()
{
	//インスタンス破棄.
	for (int i = enList::max - 1; i >= 0; i--)
	{
		SAFE_DELETE(m_pSkinMesh[i]);
	}
}

bool CSkinManager::Load()
{

	struct SoundList
	{
		int listNo;				//enList列挙型を設定.
		const TCHAR path[256];	//ファイルの名前(パス付き).
	};
	SoundList SList[] =
	{
		{ enList::Kaito,	_T("Data\\Mesh\\Skin\\Player\\kaito\\kaito_s.x") },
		{ enList::Zako,		_T("Data\\Mesh\\Skin\\Zako\\zako.x") },
		{ enList::Zako2,	_T("Data\\Mesh\\Skin\\ZAKO2\\ZAKO2_0.x") },
		{ enList::Boss,		_T("Data\\Mesh\\Skin\\Robot\\tm_f22_robo_anim_Black.X") },

	};
	//配列の最大要素数を算出 (配列全体のサイズ/配列1つ分のサイズ).
	int list_max = sizeof(SList) / sizeof(SList[0]);
	for (int i = 0; i < list_max; i++)
	{
		if (m_pSkinMesh[SList[i].listNo]->Init(SList[i].path))
		{
			return false;
		}
	}

	return true;
}
