#include "CEnemy.h"

CEnemy::CEnemy()
	: m_Motion			()

	, m_PlayerLength	()
	, m_TargetPos		()
	, m_TargetLength	()

	, m_RandomMotion	()

	, m_IsDie			()
	, m_IfRandOfGC		(false)
{
}

CEnemy::~CEnemy()
{
}

void CEnemy::Init()
{
}

void CEnemy::Update()
{
}

void CEnemy::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CSkinMeshObject::Draw(View, Proj, Light, Camera);
}

void CEnemy::DrawShadowMap()
{
	CSkinMeshObject::DrawShadowMap();

}

void CEnemy::QuaterDraw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CSkinMeshObject::QuaterDraw(View, Proj, Light, Camera);
}

void CEnemy::RayUpdate()
{
	//���C�̈ʒu���v���C���[�̍��W�ɂ��낦��.
	m_pRayY->Position = m_vPosition;
	//�n�ʂɂ߂荞�݉���̂��߁A�v���C���[�̈ʒu����������ɂ��Ă���.
	m_pRayY->Position.y += 0.5f;
	m_pRayY->RotationY = m_vRotation.y;

	//�\��(�O�㍶�E�ɐL�΂���)���C�̐ݒ�.
	for (int dir = 0; dir < CROSSRAY::max; dir++)
	{
		m_pCrossRay->Ray[dir].Position = m_vPosition;
		m_pCrossRay->Ray[dir].Position.y += 0.4f;
		m_pCrossRay->Ray[dir].RotationY = m_vRotation.y;
	}
}

void CEnemy::TargetPosSet(float radius)
{
	m_PlayerLength = CKaito::GetInstance()->GetPosition() - m_vPosition;

	D3DXVec3Normalize(&m_PlayerLength, &m_PlayerLength);


	m_TargetPos = CKaito::GetInstance()->GetPosition() - m_PlayerLength * radius;
}

void CEnemy::RotationToTarget(const D3DXVECTOR3& TargetPos, float RotAdjust)
{
	static float angle;
	angle = atan2f(m_vPosition.x - TargetPos.x
		, m_vPosition.z - TargetPos.z);

	m_vRotation.y = angle + RotAdjust;
}
