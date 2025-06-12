#include "CSprite3DManager.h"

CSprite3DManager::CSprite3DManager()
	: m_pSprite3D	()
{
	//インスタンス生成.
	for (int i = 0; i < enList::max; i++)
	{
		m_pSprite3D[i] = new CSprite3D();
	}
}

CSprite3DManager::~CSprite3DManager()
{
	//インスタンス破棄.
	for (int i = enList::max - 1; i >= 0; i--)
	{
		SAFE_DELETE(m_pSprite3D[i]);
	}
}

bool CSprite3DManager::Load()
{
	//Hpスプライトの構造体.
	CSprite3D::SPRITE_STATE SS_Hp = { 1.0f, 0.3f, 500.0f, 375.0f, 500.0f, 125.0f };

	struct SoundList
	{
		int listNo;				//enList列挙型を設定.
		const TCHAR path[256];	//ファイルの名前(パス付き).
		CSprite3D::SPRITE_STATE ss;	//スプライトステート
	};
	SoundList SList[] =
	{
		{ enList::Hp,	_T("Data\\Texture\\Hp.png"), SS_Hp },



	};
	//配列の最大要素数を算出 (配列全体のサイズ/配列1つ分のサイズ).
	int list_max = sizeof(SList) / sizeof(SList[0]);
	for (int i = 0; i < list_max; i++)
	{
		if (m_pSprite3D[SList[i].listNo]->Init(SList[i].path, SList[i].ss))
		{
			return false;
		}
	}

	return true;
}
