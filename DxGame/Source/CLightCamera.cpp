#include "CLightCamera.h"

CLightCamera::CLightCamera()
	: m_Light	()
	, m_Camera	()
{
	m_Light.vDirection = D3DXVECTOR3(1.0f, 1.5f, -1.0f);	//ライト方向.
	m_Light.Position = D3DXVECTOR3(5.0f, 30.0f, -10.0f);	//ライト方向.

	m_Camera.Position = m_Light.Position;
	m_Camera.Look	  = m_Light.vDirection;

}

CLightCamera::~CLightCamera()
{
}

void CLightCamera::Update()
{
	m_Camera.Position = m_Light.Position;
	m_Camera.Look = m_Light.vDirection;

}

void CLightCamera::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, CAMERA& Camera)
{
	D3DXVECTOR3 cam_pos = m_Camera.Position;
	D3DXVECTOR3 cam_look = m_Camera.Look;
	D3DXVECTOR3	vUpVec(0.0f, 10.0f, 0.0f);	//上方（ベクトル）.

	//ビュー（カメラ）変換.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&cam_pos, &cam_look, &vUpVec);

	D3DXMatrixOrthoLH(
		&m_mProj,		//結果計算
		Proj_W / 3,			//幅
		Proj_H / 3,			//高さ
		0.1f,			//近いビュー平面のｚ値
		FarClip * 10	//遠いビュー平面のｚ値
	);
}
