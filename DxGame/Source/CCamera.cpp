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
		m_Camera.Position.y += 5.0f; //カメラのｙ座標の位置を固定

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
	D3DXVECTOR3	vUpVec(0.0f, 100.0f, 0.0f);	//上方（ベクトル）.

	//ビュー（カメラ）変換.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&cam_pos, &cam_look, &vUpVec);



	//y方向の視野角。数値を大きくしたら視野が狭くなる.
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));	//ラジアン値.
	//アスペクト（幅÷高さ）.
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 100.0f;

	//プロジェクション（射影）変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)プロジェクション計算結果.
		fov_y,		//視野角（FOV：Field of View）.
		aspect,		//アスペクト.
		near_z,		//近いビュー平面のz値.
		far_z);	//遠いビュー平面のz値.

}


//三人称視点カメラ
void CCamera::ThirdCamera()
{

	//注視点が移動した場合カメラの水平角度を考慮した座標に移動する
	if (MoveFlag == true)
	{

		fsin = sinf(m_CameraX / SemiCircle * D3DX_PI);
		fcos = cosf(m_CameraX / SemiCircle * D3DX_PI);
		m_tempVector.x = m_Player.x * fcos - m_Player.z * fsin;
		m_tempVector.y = 0.0f;
		m_tempVector.z = m_Player.x * fsin - m_Player.z * fcos;
		m_Player += m_tempVector;

	}

	//カメラの位置と向きを算出
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
	//Z軸ベクトル
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);

	//y方向の回転行列.
	D3DXMATRIX mRotationY;
	//Y軸回転行列を作成.
	D3DXMatrixRotationY(
		&mRotationY,	//(out)行列.
		TargetRotY);	//対象のY方向の回転値.

	//Y軸回転行列を使ってZ軸ベクトルを座標変化する.
	D3DXVec3TransformCoord(
		&vecAxisZ,		//(out)Z軸ベクトル.
		&vecAxisZ,		//(in)Z軸ベクトル.
		&mRotationY);	//Y軸回転行列.

	//カメラの位置、注視点を対象にそろえる.
	pCamera->Position = TargetPos;
	pCamera->Look = TargetPos;

	//カメラの位置、注視点をZ軸ベクトルを用いて調整.
	pCamera->Position -= vecAxisZ * 4.0f;	//対象の背中側.
	pCamera->Look += vecAxisZ * 3.0f;		//対象を挟んで向こう側.

	//カメラの位置、注視点の高さをそれぞれ微調整.
	pCamera->Position.y += 2.0f;
	pCamera->Look.y += 0.5f;


}

//マウス操作
void CCamera::MouseCursor(float max_y, float min_y)
{
	if (GetAsyncKeyState(VK_F1) & 0x0001)
	{
		m_MouseDrag = !m_MouseDrag;	//F1が押されたら切り替える
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

	return center;	//スクリーン中央の座標を返す
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

