#pragma once
#include "CSceneBase.h"
#include "CSceneManager.h"

//このクラス以外のシーンのヘッダーをインクルード
#include "CSceneGameOver.h"
#include "CSceneGameClear.h"

/**************フェーズ***************/
#include "CPhaseManager.h"
#include "CPhase1.h"
#include "CPhase2.h"
#include "CFinalPhase.h"

/**************************************


/**************************************
*		ゲームメインシーン
**/

class CSceneMain
	: public CSceneBase
{
public:
	CSceneMain();
	~CSceneMain() override;

	void Init() override;

	void Update() override;

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void Exit() override;

protected:
	CSceneManager*	m_pSceneManager;

	std::unique_ptr<CUIObject>		m_pUIOpeInstruct;
	std::unique_ptr<CUIObject>		m_pUIOpeInstructSub;

	bool							m_ChangeSceneTrigger;

};

