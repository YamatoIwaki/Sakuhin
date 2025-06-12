#include "CSprite2DManager.h"

CSprite2DManager::CSprite2DManager()
	: m_pSprite2D()
{
	//インスタンス生成.
	for (int i = 0; i < enList::max; i++)
	{
		m_pSprite2D[i] = new CSprite2D();
	}
}

CSprite2DManager::~CSprite2DManager()
{
	//インスタンス破棄.
	for (int i = enList::max - 1; i >= 0; i--)
	{
		SAFE_DELETE(m_pSprite2D[i]);
	}
}

bool CSprite2DManager::Load()
{

	//2Dテクスチャの構造体
	CSprite2D::SPRITE_STATE SS_K_HpG			= { 300.0f, 50.0f, 500.0f, 375.0f, 500.0f, 125.0f };
	CSprite2D::SPRITE_STATE SS_K_HpT			= { 100.0f, 50.0f, 250.0f, 125.0f, 250.0f, 125.0f };
	CSprite2D::SPRITE_STATE SS_B_HpG			= { 600.0f, 50.0f, 500.0f, 250.0f, 500.0f - 0.5f , 125.0f };
	CSprite2D::SPRITE_STATE SS_B_HpT			= { 200.0f, 75.0f, 300.0f, 125.0f, 300.0f, 125.0f };
	CSprite2D::SPRITE_STATE SS_Click			= { 800.0f, 400.0f, 800.0f, 400.0f, 800.0f, 400.0f };
	CSprite2D::SPRITE_STATE SS_Title			= { 800.0f, 400.0f, 800.0f, 400.0f, 800.0f, 400.0f };
	CSprite2D::SPRITE_STATE SS_GameClear		= { 800.0f, 400.0f, 800.0f, 400.0f, 800.0f, 400.0f };
	CSprite2D::SPRITE_STATE SS_GameOver			= { 800.0f, 400.0f, 800.0f, 400.0f, 800.0f, 400.0f };
	CSprite2D::SPRITE_STATE SS_FullScr			= { WND_W, WND_H, WND_W, WND_H, WND_W, WND_H };
	CSprite2D::SPRITE_STATE SS_Phase1or2		= { 600.0f, 150.0f, 600.0f, 150.0f, 600.0f, 150.0f };
	CSprite2D::SPRITE_STATE SS_FinalPhase		= { 600.0f, 300.0f, 600.0f, 300.0f, 600.0f, 300.0f };
	CSprite2D::SPRITE_STATE SS_OpeInstruct		= { 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f };
	CSprite2D::SPRITE_STATE SS_OpeInstructSub	= { 130.0f,  75.0f, 130.0f,  75.0f, 130.0f,  75.0f };
	CSprite2D::SPRITE_STATE SS_Start			= { 1000.0f, 250.0f, 600.0f, 150.0f, 600.0f, 150.0f };

	struct SoundList
	{
		int listNo;				//enList列挙型を設定.
		const TCHAR path[256];	//ファイルの名前(パス付き).
		CSprite2D::SPRITE_STATE ss;	//スプライトステート
	};
	SoundList SList[] =
	{
		{ enList::KaitoHp,			_T("Data\\Texture\\Hp.png"),					SS_K_HpG },
		{ enList::BossHp,			_T("Data\\Texture\\BossHp.png"),				SS_B_HpG },
		{ enList::FadeBlack,		_T("Data\\Texture\\Black.png"),					SS_FullScr },
		{ enList::OpeInstruct,		_T("Data\\Texture\\OpeInstruct.png"),			SS_OpeInstruct },
		{ enList::OpeInstructSub,	_T("Data\\Texture\\OpeInstructSub.png"),		SS_OpeInstructSub },


		{ enList::KaitoHpText,		_T("Data\\Texture\\TextHp.png"),				SS_K_HpT },
		{ enList::BossHpText,		_T("Data\\Texture\\BossTextHp.png"),			SS_B_HpT },
		{ enList::ClickText,		_T("Data\\Texture\\Click.png"),					SS_Click },
		{ enList::TitleText,		_T("Data\\Texture\\CleanUpTitle.png"),			SS_Title },
		{ enList::GameClearText,	_T("Data\\Texture\\GameClear.png"),				SS_GameClear },
		{ enList::GameOverText,		_T("Data\\Texture\\GameOver.png"),				SS_GameOver },
		{ enList::Phase1Text,		_T("Data\\Texture\\Phase1Text.png"),			SS_Phase1or2 },
		{ enList::Phase2Text,		_T("Data\\Texture\\Phase2Text.png"),			SS_Phase1or2 },
		{ enList::FinalPhaseText,	_T("Data\\Texture\\FinalPhaseText.png"),		SS_FinalPhase },
		{ enList::StartText,		_T("Data\\Texture\\StartText.png"),				SS_Start },
		{ enList::LoadingText,		_T("Data\\Texture\\Loading.png"),				SS_FullScr },


	};
	//配列の最大要素数を算出 (配列全体のサイズ/配列1つ分のサイズ).
	int list_max = sizeof(SList) / sizeof(SList[0]);
	for (int i = 0; i < list_max; i++)
	{
		if (m_pSprite2D[SList[i].listNo]->Init(
			SList[i].path,
			SList[i].ss))
		{
			return false;
		}
	}

	return true;
}
