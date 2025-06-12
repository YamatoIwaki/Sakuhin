#pragma once
#pragma once
#include "CZako2Hp.h"

/*******************************************
*		ザコ2の体力赤ゲージ
**/


class CZako2BackGauge
	: public CZako2Hp
{
public:
	CZako2BackGauge();
	~CZako2BackGauge();

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) override;



protected:

};
