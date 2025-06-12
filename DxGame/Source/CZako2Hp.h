#pragma once
#pragma once
#include "CSpriteObject.h"
#include "CZako2.h"

/*******************************************
*		ÉUÉR2ÇÃhp
**/


class CZako2Hp
	: public CSpriteObject
{
public:
	const D3DXVECTOR3 AddUp = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
public:
	CZako2Hp();
	~CZako2Hp();

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) override;

	void SetZako2Pos(D3DXVECTOR3 zakoPos) { m_vPosition = zakoPos + AddUp; }

	void SetZako2Hp(float hp) { m_Zako2Hp = hp; }

protected:
	float		m_Zako2Hp;

};

