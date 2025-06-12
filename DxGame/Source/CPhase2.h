#pragma once
#include "CPhaseBase.h"
#include "CPhaseManager.h"
#include "CFinalPhase.h"

/******************************************
*			�t�F�[�Y2�N���X
**/

class CPhase2
	: public CPhaseBase

{
public:
	CPhase2();
	~CPhase2() override;


	void Init() override;

	void Update() override;

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void DrawShadow() override;

	void Exit() override;


private:

};
