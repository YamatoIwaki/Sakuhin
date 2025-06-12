#include "CApperGround.h"

CApperGround::CApperGround()
{
	m_vScale *= 2.5f;
}

CApperGround::~CApperGround()
{
}

void CApperGround::Init()
{
}

void CApperGround::Update()
{


}

void CApperGround::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}
