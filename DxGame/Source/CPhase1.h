#pragma once
#include "CPhaseBase.h"
#include "CPhaseManager.h"
#include "CPhase2.h"

/******************************************
*			フェーズ１クラス
**/

class CPhase1
	: public CPhaseBase

{
public:
	CPhase1();
	~CPhase1() override;


	void Init() override;

	void Update() override;

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void DrawShadow() override;

	void Exit() override;


private:




};
