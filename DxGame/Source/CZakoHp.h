#pragma once
#include "CSpriteObject.h"
#include "CZako.h"

/*******************************************
*		ÉUÉRÇÃhp
**/


class CZakoHp
	: public CSpriteObject
{
public:
	const D3DXVECTOR3 AddUp = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
public:
	CZakoHp();
	~CZakoHp();

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) override;

	void SetZakoPos(D3DXVECTOR3 zakoPos) { m_vPosition = zakoPos + AddUp; }
	
	void SetZakoHp(float hp) { m_ZakoHp = hp; }

protected:
	float		m_ZakoHp;

};

