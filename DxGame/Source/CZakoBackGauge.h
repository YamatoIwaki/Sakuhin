#pragma once
#include "CZakoHp.h"

/*******************************************
*		�U�R�̗̑͐ԃQ�[�W
**/


class CZakoBackGauge
	: public CZakoHp
{
public:
	CZakoBackGauge();
	~CZakoBackGauge();

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) override;



protected:

};
