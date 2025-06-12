#pragma once
#include "CSingletonManager.h"

/****************************/
/*       カメラクラス        */
//**************************//

#define Camera_Distance 10.0f

#define MAX_Y			20.0f
#define MIN_Y			-50.0f

#define Circle          360.0f
#define SemiCircle		180.0f

class CCamera
	: public SingletonManager<CCamera>
{
public:
	static constexpr float WalkSpeed	= 0.02f;
	static constexpr float ShakeAmp		= 0.005f;
	static constexpr float ShakeFreq	= 6.0f;
	static constexpr float RollAmp		= 0.05f;

	static constexpr float Magnitude	= 0.1f;
	static constexpr float BossFreq		= 20.0f;
	static constexpr float VibTime		= 8.0f;

	static constexpr float MouseCamSp	= 0.1f;	//マウスカメラのスピード


public:
	friend class SingletonManager<CCamera>;

	void Update(enScene Scene);

	void Draw();

public:

	//三人称視点カメラ
	void ThirdCamera();

	//三人称カメラ(対象の角度と合わせる)
	void ThirdPersonCamera(CAMERA* pCamera,
		const D3DXVECTOR3& TargetPos, float TargetRotY);

	//マウス操作
	void MouseCursor(float max_y, float min_y);

	POINT GetWindowCenter();

	//歩いている風カメラ
	void WalkCamera();

	//画面揺れカメラ
	void VibCamera(float vibTime, float spacetime);


public:

	//ウィンドウ情報を取得
	void SethWnd(HWND hWnd){
		m_hWnd = hWnd;
	}

	void SetViewPos(const D3DXVECTOR3& View) {
		m_Player = View;
	};

	void SetViewRot(const D3DXVECTOR3& Rot) {
		m_PlayerRot = Rot;
	};

	void SetMoveFlag(const bool MF)
	{
		MoveFlag = MF;
	}

	D3DXMATRIX GetViewMatrix() {
		return m_mView;
	}

	D3DXMATRIX GetProjMatrix() {
		return m_mProj;
	}

	D3DXVECTOR3 GetPosition() { return m_Camera.Position; }

	D3DXVECTOR3 GetLook() { return m_Camera.Look; }

	CAMERA GetCamera() { return m_Camera; }

	void SetLook(const D3DXVECTOR3& Look )
	{
		m_Camera.Look = Look;
	}

	void ResetPos() {
		m_CameraX = 0;
		m_CameraY = 0;
		m_CameraZ = 0;
	}

private:
	CCamera();
	~CCamera();

	//ウィンドウ情報
	HWND			m_hWnd;

	//カメラ情報.
	CAMERA			m_Camera;

	//行列.
	D3DXMATRIX		m_mView;	//ビュー(カメラ)行列.
	D3DXMATRIX		m_mProj;	//射影行列


	//注視点
	D3DXVECTOR3		m_Player;
	D3DXVECTOR3		m_PlayerRot;

	//注視点の動いているか判定
	bool			MoveFlag;

	//臨時座標
	D3DXVECTOR3		m_tempVector;
	D3DXVECTOR3		m_vtempPos1;
	D3DXVECTOR3		m_vtempPos2;

	//カメラ操作
	float	m_CameraX;
	float	m_CameraY;
	float	m_CameraZ;

	//三人称カメラ
	float	fsin;
	float	fcos;

	//Vib用
	float	m_VibCnt;
	float	m_SpaceCnt;
	bool	m_IsVib;
	bool	m_IsGetPos;

	D3DXVECTOR3	m_DefaPos;

	//マウスポインタ
	POINT	m_MousePoint;

	bool	m_MouseDrag;


};

