#include "CLightCamera.h"

CLightCamera::CLightCamera()
	: m_Light	()
	, m_Camera	()
{
	m_Light.vDirection = D3DXVECTOR3(1.0f, 1.5f, -1.0f);	//���C�g����.
	m_Light.Position = D3DXVECTOR3(5.0f, 30.0f, -10.0f);	//���C�g����.

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
	D3DXVECTOR3	vUpVec(0.0f, 10.0f, 0.0f);	//����i�x�N�g���j.

	//�r���[�i�J�����j�ϊ�.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)�r���[�v�Z����.
		&cam_pos, &cam_look, &vUpVec);

	D3DXMatrixOrthoLH(
		&m_mProj,		//���ʌv�Z
		Proj_W / 3,			//��
		Proj_H / 3,			//����
		0.1f,			//�߂��r���[���ʂ̂��l
		FarClip * 10	//�����r���[���ʂ̂��l
	);
}
