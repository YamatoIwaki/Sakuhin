#pragma once
#include "CStaticMeshObject.h"

/*******************************************
*	ƒUƒR‚Q‚ÌUŒ‚‚Ì“–‚½‚è”»’è
**/


class CZako2AttackHitPoint
	: public CStaticMeshObject
{
public:
	CZako2AttackHitPoint();
	virtual ~CZako2AttackHitPoint() override;

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	bool GetIsHitAttack() { return m_IsHitAttack; }

	void SetIsHitAttack(bool IsHit) { m_IsHitAttack = IsHit; }

	void SetTargetPos(D3DXVECTOR3 Pos) { m_TargetPos = Pos; }

	void SetTargetRot(float Rot) { m_TargetRotY = Rot; }

private:

	bool			m_IsHitAttack;		//“–‚½‚Á‚½‚©‚Ì”»’è

	D3DXVECTOR3		m_TargetPos;		//‘ÎÛ‚Ì‰ñ“]’l	

	float			m_TargetRotY;		//‘ÎÛ‚Ì‰ñ“]’l	

};

