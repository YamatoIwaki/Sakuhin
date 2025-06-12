#pragma once
#include "CSingletonManager.h"

/****************************/
/*    ���C�g�J�����N���X      */
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
	//�s��.
	D3DXMATRIX		m_mProj;	//�ˉe�s��
	D3DXMATRIX		m_mView;	//�ˉe�s��


	CAMERA			m_Camera;

	LIGHT			m_Light;


};

