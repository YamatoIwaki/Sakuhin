#pragma once
#include "CSprite2D.h"
#include "CSingletonManager.h"

/*********************************************************
*			スプライト2Dマネージャー
**/




class CSprite2DManager
	: public SingletonManager<CSprite2DManager>
{
public:
	enum enList
	{
		KaitoHp,		//カイトHp
		BossHp,			//ボスHp
		FadeBlack,		//fade用の黒背景
		OpeInstruct,	//操作説明用画像
		OpeInstructSub,	//操作説明用画像(サブ)

		KaitoHpText,	//カイトHpテキスト
		BossHpText,		//ボスHpテキスト
		ClickText,		//Clickテキスト
		TitleText,		//タイトルテキスト
		GameClearText,	//ゲームクリアテキスト
		GameOverText,	//ゲームオーバーテキスト
		Phase1Text,		//フェーズ１
		Phase2Text,		//フェーズ２
		FinalPhaseText,	//ファイナルフェーズ
		StartText,		//スタートテキスト
		LoadingText,	//
		//追加したら上に追加
		max,
	};


private:
	CSprite2DManager();
public:
	friend class SingletonManager<CSprite2DManager>;

	~CSprite2DManager();

	//データ読込関数.
	bool Load();


	static CSprite2D* GetSprite(enList list) { return CSprite2DManager::GetInstance()->m_pSprite2D[list]; }


private:

	CSprite2D* m_pSprite2D[enList::max];

};

