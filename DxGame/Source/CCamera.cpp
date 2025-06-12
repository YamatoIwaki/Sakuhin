#include "CCamera.h"
#include "CSoundManager.h"

CCamera::CCamera()
	: m_hWnd		()
	
	, m_Camera		()

	, m_mView		()

	, MoveFlag		()

	, m_CameraX		()
	, m_CameraY		()
	, m_CameraZ		()

	, fsin			()
	, fcos			()

	, m_VibCnt		()
	, m_SpaceCnt	()
	, m_IsVib		()
	, m_IsGetPos	()
	, m_DefaPos		()

	, m_MousePoint	()
	, m_MouseDrag	(false)
{
}

CCamera::~CCamera()
{
}

void CCamera::Update(enScene Scene)
{
	switch (Scene)
	{
	case Title:
	case GameOver:
	case GameClear:
	{
		m_Camera.Look = D3DXVECTOR3(0.0f, 1.0f, 4.0f);
		m_Camera.Position = D3DXVECTOR3(0.0f, 10.0f, -20.0f);
		while(ShowCursor(TRUE) < 0);

	}
		break;
	case GameMain:
	{
		m_Camera.Look = m_Player + D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_Camera.Position = m_vtempPos2 + m_Player;
		m_Camera.Position.y += 5.0f; //�J�����̂����W�̈ʒu���Œ�

		MouseCursor(MAX_Y, MIN_Y);

		ThirdCamera();
	}
		break;
	}

}



void CCamera::Draw()
{
	D3DXVECTOR3 cam_pos = m_Camera.Position;
	D3DXVECTOR3 cam_look = m_Camera.Look;
	D3DXVECTOR3	vUpVec(0.0f, 100.0f, 0.0f);	//����i�x�N�g���j.

	//�r���[�i�J�����j�ϊ�.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)�r���[�v�Z����.
		&cam_pos, &cam_look, &vUpVec);



	//y�����̎���p�B���l��傫�������王�삪�����Ȃ�.
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));	//���W�A���l.
	//�A�X�y�N�g�i���������j.
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 100.0f;

	//�v���W�F�N�V�����i�ˉe�j�ϊ�.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)�v���W�F�N�V�����v�Z����.
		fov_y,		//����p�iFOV�FField of View�j.
		aspect,		//�A�X�y�N�g.
		near_z,		//�߂��r���[���ʂ�z�l.
		far_z);	//�����r���[���ʂ�z�l.

}


//�O�l�̎��_�J����
void CCamera::ThirdCamera()
{

	//�����_���ړ������ꍇ�J�����̐����p�x���l���������W�Ɉړ�����
	if (MoveFlag == true)
	{

		fsin = sinf(m_CameraX / SemiCircle * D3DX_PI);
		fcos = cosf(m_CameraX / SemiCircle * D3DX_PI);
		m_tempVector.x = m_Player.x * fcos - m_Player.z * fsin;
		m_tempVector.y = 0.0f;
		m_tempVector.z = m_Player.x * fsin - m_Player.z * fcos;
		m_Player += m_tempVector;

	}

	//�J�����̈ʒu�ƌ������Z�o
	fsin = sinf(m_CameraY / SemiCircle * D3DX_PI);
	fcos = cosf(m_CameraY / SemiCircle * D3DX_PI);
	m_vtempPos1.x = 0.0f;
	m_vtempPos1.y = fsin * Camera_Distance;
	m_vtempPos1.z = -fcos * Camera_Distance;

	fsin = sinf(m_CameraX / SemiCircle * D3DX_PI);
	fcos = cosf(m_CameraX / SemiCircle * D3DX_PI);
	m_vtempPos2.x = fcos * m_vtempPos1.x - fsin * m_vtempPos1.z;
	m_vtempPos2.y = m_vtempPos1.y;
	m_vtempPos2.z = fsin * m_vtempPos1.x + fcos * m_vtempPos1.z;

}

void CCamera::ThirdPersonCamera(CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY)
{
	//Z���x�N�g��
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);

	//y�����̉�]�s��.
	D3DXMATRIX mRotationY;
	//Y����]�s����쐬.
	D3DXMatrixRotationY(
		&mRotationY,	//(out)�s��.
		TargetRotY);	//�Ώۂ�Y�����̉�]�l.

	//Y����]�s����g����Z���x�N�g�������W�ω�����.
	D3DXVec3TransformCoord(
		&vecAxisZ,		//(out)Z���x�N�g��.
		&vecAxisZ,		//(in)Z���x�N�g��.
		&mRotationY);	//Y����]�s��.

	//�J�����̈ʒu�A�����_��Ώۂɂ��낦��.
	pCamera->Position = TargetPos;
	pCamera->Look = TargetPos;

	//�J�����̈ʒu�A�����_��Z���x�N�g����p���Ē���.
	pCamera->Position -= vecAxisZ * 4.0f;	//�Ώۂ̔w����.
	pCamera->Look += vecAxisZ * 3.0f;		//�Ώۂ�����Ō�������.

	//�J�����̈ʒu�A�����_�̍��������ꂼ�������.
	pCamera->Position.y += 2.0f;
	pCamera->Look.y += 0.5f;


}

//�}�E�X����
void CCamera::MouseCursor(float max_y, float min_y)
{
	if (GetAsyncKeyState(VK_F1) & 0x0001)
	{
		m_MouseDrag = !m_MouseDrag;	//F1�������ꂽ��؂�ւ���
	}

	if (m_MouseDrag == false)
	{
		while (ShowCursor(FALSE) >= 0);
		GetCursorPos(&m_MousePoint);
		ScreenToClient(m_hWnd, &m_MousePoint);

		POINT center = GetWindowCenter();;
		float dx = (m_MousePoint.x - center.x) * MouseCamSp;
		float dy = (m_MousePoint.y - center.y) * MouseCamSp;

		m_CameraX -= dx;
		m_CameraY += dy;

		if (m_CameraY >= max_y)
		{
			m_CameraY = max_y;
		}
		if (m_CameraY <= min_y)
		{
			m_CameraY = min_y;
		}

		ClientToScreen(m_hWnd, &center);

		SetCursorPos(center.x, center.y);
	}
	else
	{
		while (ShowCursor(TRUE) < 0);
	}
}

POINT CCamera::GetWindowCenter()
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	POINT center;
	center.x = (rect.right - rect.left) / 2;
	center.y = (rect.bottom - rect.top) / 2;

	return center;	//�X�N���[�������̍��W��Ԃ�
}

void CCamera::WalkCamera()
{
	float time = GetTickCount64() / 1000.0f;

	m_CameraY = ShakeAmp * sinf(time * ShakeFreq);

	m_Camera.Position += D3DXVECTOR3(0.0f, m_CameraY, m_CameraZ);

	float cameraRoll = RollAmp * cosf(time * ShakeFreq);

	m_Camera.Look.z += cameraRoll;


}

void CCamera::VibCamera(float vibTime, float spacetime)
{

	m_SpaceCnt += SECOND;
	if (m_SpaceCnt > spacetime)
	{
		m_SpaceCnt = 0.0f;
		m_IsVib = true;
	}

	if (m_IsVib == true)
	{
		m_VibCnt += 0.1f;

		if (m_IsGetPos == false)
		{
			m_DefaPos = m_Camera.Position;
			m_IsGetPos = true;
		}

		if (vibTime < m_VibCnt)
		{
			m_VibCnt = 0.0f;
			m_Camera.Position = m_DefaPos;
			m_IsGetPos = false;
			m_IsVib = false;
		}

		float VibSize = sinf(m_VibCnt * BossFreq) * Magnitude;

		m_Camera.Position.y += VibSize;
		m_Camera.Position.x += VibSize;

	}
}

