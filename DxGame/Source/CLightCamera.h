#pragma once
#include "CSingletonManager.h"

/****************************/
/*    ライトカメラクラス      */
//**************************//

class CLightCamera
	: public SingletonManager<CLightCamera>
{
	
public:
	static constexpr float Proj_W	= WND_W;
	static constexpr float Proj_H	= WND_H;
	static constexpr float FarClip	= 100.0f;

private:
	CLightCamera();
	~CLightCamera() ;
public:
	friend class SingletonManager<CLightCamera>;

	void Update() ;

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, CAMERA& Camera) ;

	LIGHT& GetLight() {
		return m_Light;
	}

	CAMERA	GetCamera() { return m_Camera; }

	D3DXMATRIX GetViewMatrix() {
		return m_mView;
	}

	D3DXMATRIX GetProjMatrix() {
		return m_mProj;
	}

private:
	//行列.
	D3DXMATRIX		m_mProj;	//射影行列
	D3DXMATRIX		m_mView;	//射影行列


	CAMERA			m_Camera;

	LIGHT			m_Light;


};

