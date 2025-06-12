#pragma once

#include "CStaticMeshObject.h"

/**************************************************
*	�n�ʂ̌����ڃN���X.
**/
class CApperGround
	: public CStaticMeshObject
{
public:

	CApperGround();

	virtual ~CApperGround();

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

protected:

};

