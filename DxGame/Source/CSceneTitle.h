#pragma once
#include "CSceneBase.h"
#include "CSceneManager.h"

//このクラス以外のシーンのヘッダーをインクルード
#include "CSceneMain.h"

/**************************
*		タイトルシーンクラス
**/

class CSceneTitle
	: public CSceneBase
{
public:
	CSceneTitle();
	~CSceneTitle() override;

	void Init() override;

	void Update() override;

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void Exit() override;
private:


private:
	CSceneManager* m_pSceneManager;

	std::unique_ptr<CUIObject>			m_pTitleText;

};

