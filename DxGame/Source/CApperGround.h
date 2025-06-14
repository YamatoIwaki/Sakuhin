#pragma once

#include "CStaticMeshObject.h"

/**************************************************
*	地面の見た目クラス.
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

