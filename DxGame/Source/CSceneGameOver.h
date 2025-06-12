#pragma once
#pragma once
#include "CSceneBase.h"
#include "CSceneManager.h"

//このクラス以外のシーンのヘッダーをインクルード
#include "CSceneTitle.h"

/**************************
*		ゲームオーバーシーンクラス
**/

class CSceneGameOver
	: public CSceneBase
{
public:
	CSceneGameOver();
	~CSceneGameOver() override;

	void Init() override;

	void Update() override;

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void Exit() override;

private:
	CSceneManager*	m_pSceneManager;

	std::unique_ptr<CUIObject>				m_pGameOverText;


};

