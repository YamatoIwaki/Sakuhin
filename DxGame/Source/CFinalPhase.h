#pragma once
#include "CPhaseBase.h"
#include "CPhaseManager.h"

/******************************************
*			ファイナルフェーズクラス
**/

class CFinalPhase
	: public CPhaseBase

{
public:
	CFinalPhase();
	~CFinalPhase() override;


	void Init() override;

	void Update() override;

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void DrawShadow() override;

	void Exit() override;


private:

};
