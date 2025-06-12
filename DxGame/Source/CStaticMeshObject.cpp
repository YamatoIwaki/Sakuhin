#include "CStaticMeshObject.h"

CStaticMeshObject::CStaticMeshObject()
	: m_pMesh		( nullptr )
{
}

CStaticMeshObject::~CStaticMeshObject()
{
	DetachMesh();
}

void CStaticMeshObject::Init()
{
}

void CStaticMeshObject::Update()
{
	if( m_pMesh == nullptr ){
		return;
	}
}

void CStaticMeshObject::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj,
	LIGHT& Light, CAMERA& Camera )
{
	if( m_pMesh == nullptr ){
		return;
	}

	//�`�撼�O�ō��W���]���Ȃǂ��X�V.
	m_pMesh->SetPosition( m_vPosition );
	m_pMesh->SetRotation( m_vRotation );
	m_pMesh->SetScale( m_vScale );

	//�����_�����O.
	m_pMesh->Render( View, Proj, Light, Camera.Position );
}


void CStaticMeshObject::DrawShadowMap()
{
	if (m_pMesh == nullptr) {
		return;
	}

	//�`�撼�O�ō��W���]���Ȃǂ��X�V.
	m_pMesh->SetPosition(m_vPosition);
	m_pMesh->SetRotation(m_vRotation);
	m_pMesh->SetScale(m_vScale);

	//�����_�����O.
	m_pMesh->ShadowRender();
}

bool CStaticMeshObject::IsHitForRay(const RAY& pRay, float* pfDistance, D3DXVECTOR3* pvIntersect, D3DXVECTOR3* pNormal)
{
	D3DXVECTOR3 vAxis;	//���x�N�g��,
	D3DXMATRIX	mRotationY;	//������]�s��.

	//������]�s��.
	D3DXMatrixRotationY(&mRotationY, pRay.RotationY);
	//������]�s����g���Ď��x�N�g���̍��W�ϊ�����.
	D3DXVec3TransformCoord(&vAxis, &pRay.Axis, &mRotationY);

	//���C�̎��_�ƏI�_.
	D3DXVECTOR3 StartPoint, EndPoint;
	StartPoint = pRay.Position;
	EndPoint = pRay.Position + (vAxis * pRay.Length);	//���C�̏I�_��ݒ�.

	//���C�𓖂Ă������b�V�����ړ����Ă���ꍇ�ł�.
	//�Ώۂ�world�s��̋t�s���p����ΐ��������C��������.
	D3DXMATRIX mWorld, mInverseWorld;
	//�ړ�����.
	D3DXMATRIX mTran;
	D3DXMatrixTranslation(
		&mTran,
		m_vPosition.x, m_vPosition.y, m_vPosition.z
	);

	//��]����.
	//�����̍s��v�Z��[CStaticMesh::Render()�֐�]�Ɠ�������K�v����.
	D3DXMATRIX mRot, mYaw, mPitch, mRoll;
	//Y����]�s��쐬.
	D3DXMatrixRotationY(&mYaw, m_vRotation.y);
	//X����]�s��쐬.
	D3DXMatrixRotationX(&mPitch, m_vRotation.x);
	//Z����]�s��.
	D3DXMatrixRotationZ(&mRoll, m_vRotation.z);
	//��]�s��쐬.
	mRot = mYaw * mPitch * mRoll;

	//�g�k����.
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, m_vScale.x, m_vScale.y, m_vScale.z);

	//���[���h�s��v�Z.
	//�g�k�~��]�~�ړ� �����Ԃ��ƂĂ���؁I.
	mWorld = mScale * mRot * mTran;

	//�t�s������߂�.
	D3DXMatrixInverse(&mInverseWorld, nullptr, &mWorld);
	//���C�̎n�_�A�I�_�ɔ��f.
	D3DXVec3TransformCoord(&StartPoint, &StartPoint, &mInverseWorld);
	D3DXVec3TransformCoord(&EndPoint, &EndPoint, &mInverseWorld);

	//�����ƒ���(�傫��)�����߂�.
	D3DXVECTOR3 vDirection = EndPoint - StartPoint;

	BOOL bHit = FALSE;		//�����t���O	.
	DWORD dwIndex = 0;		//�C���f�b�N�X�ԍ�
	D3DXVECTOR3 Vertex[3];	//���_���W
	FLOAT U = 0, V = 0;		//�d�S�q�b�g���W.

	//���b�V���ƃ��C�̌����𒲂ׂ�.
	D3DXIntersect(
		m_pMesh->GetMeshForRay(),	//�Ώۃ��b�V��
		&StartPoint,				//���C�̎n�_
		&vDirection,				//���C�̌����ƒ����i�傫���j
		&bHit,						//(out)���茋��.
		&dwIndex,					//(out)bHit��TRUE���Ƀ��C�̎n�_�ɍł��߂��̖ʂ̃C���f�b�N�X�l�ւ̃|�C���^
		&U, &V,						//(out)�d�S�q�b�g���W.
		pfDistance,					//(out)���b�V���Ƃ̋���.
		nullptr, nullptr);			//

	//�����ɐL�т郌�C�̂ǂ����Ń��b�V�����������Ă�����.
	if (bHit == TRUE)
	{
		//����������.
		FindVerticesOnPoly(
			m_pMesh->GetMeshForRay(),
			dwIndex, Vertex);

		//�d�S���W�����_���Z�o.
		//���[�J����_�́@v0 + U*(v1-v0) + V+(v2-v0)�ŋ��߂�
		*pvIntersect =
			Vertex[0] + U * (Vertex[1] - Vertex[0]) + V * (Vertex[2] - Vertex[0]);

		//���f���f�[�^���u�g�k�v�u��]�v�u�ړ��v���Ă���΍s�񂪕K�v.
		D3DXVec3TransformCoord(pvIntersect, pvIntersect, &mWorld);

		//�@���̏o�͗v���������
		if (pNormal != nullptr) {
			D3DXPLANE p;
			//���̒��_(�R�_)����u���ʂ̕������𓾂�v.
			D3DXPlaneFromPoints(&p, &Vertex[0], &Vertex[1], &Vertex[2]);
			
			//���ʂ̕������̌W�����@���̐���.
			*pNormal = D3DXVECTOR3(p.a, p.b, p.c);

			//�@���͈ړ��ʌv�Z�Ɏg�p����̂ŉ�]�̂ݏ���������
			D3DXVec3TransformCoord(pNormal, pNormal, &mRot);
		}

		//EndPoint���猩��������1.0f��菬������Γ������Ă���.
		if (*pfDistance < 1.0f)
		{
			return true;	//�������Ă���.
		}
	}

	return false;	//�O��Ă���.


	return false;
}

//�ǂ���̈ʒu���v�Z����.
void CStaticMeshObject::CalculatePositionFromWall(CROSSRAY* pCrossRay)
{
	static constexpr float WSPACE = 0.5f;	//�ǂƂ̌��E����.

	FLOAT Distance;			//���C�̋���
	D3DXVECTOR3 Intersect;	//���C�̌����_.

	D3DXVECTOR3 Normal;		//�@��(�x�N�g��).
	D3DXVECTOR3 Offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	//���C�̌����ɂ�肠����ǂ܂ł̋��������߂�.���x�N�g��(�O�㍶�E).
	for (int dir = 0; dir < CROSSRAY::max; dir++)
	{
		if (IsHitForRay(pCrossRay->Ray[dir], &Distance, &Intersect, &Normal))
		{
			if (Distance <= WSPACE)
			{
				//�@���͕ǂ���܂������ɏo�Ă���̂ŁA�@���Ƃ������킹��,
				//�Ώۂ𓮂����x�N�g����������.
				Offset = Normal * (WSPACE - Distance);

				//���C�̈ʒu���X�V.
				for (int i = 0; i < CROSSRAY::max; i++)
				{
					pCrossRay->Ray[i].Position += Offset;
				}
			}
		}
	}

}

//�����ʒu�̃|���S���̒��_��������.
//�����_���W�̓��[�J�����W.
HRESULT CStaticMeshObject::FindVerticesOnPoly(
	LPD3DXMESH pMesh,
	DWORD dwPolyIndex, 
	D3DXVECTOR3* pVertices)
{
	//���_���Ƃ̃o�C�g�����擾.
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	//���_�����擾
	DWORD dwVertexAmt = pMesh->GetNumVertices();
	//�ʐ����擾.
	DWORD dwPoly = pMesh->GetNumFaces();

	WORD* pwPoly = nullptr;

	//�C���f�b�N�X�o�b�t�@�����b�N(�ǂݍ��݃��[�h).
	pMesh->LockIndexBuffer(
		D3DLOCK_READONLY,
		reinterpret_cast<VOID**>(&pwPoly));

	BYTE* pbVertices = nullptr;	//���_(�o�C�g�^).
	FLOAT* pfVertices = nullptr;//���_(float�^).
	LPDIRECT3DVERTEXBUFFER9	VB = nullptr;	//���_�o�b�t�@.

	//���_���̎擾.
	pMesh->GetVertexBuffer( &VB );

	//���_�o�b�t�@�̃��b�N.
	if (SUCCEEDED(VB->Lock(0, 0, reinterpret_cast<VOID**> (&pbVertices), 0)))
	{
		//�|���S���̒��_��ڂ��擾.
		pfVertices = reinterpret_cast<FLOAT*>
			(&pbVertices[dwStride * pwPoly[dwPolyIndex * 3]]);

		pVertices[0].x = pfVertices[0];
		pVertices[0].y = pfVertices[1];
		pVertices[0].z = pfVertices[2];

		//�|���S���̒��_��ڂ��擾.
		pfVertices = reinterpret_cast<FLOAT*>
			(&pbVertices[dwStride * pwPoly[dwPolyIndex * 3 + 1]]);

		pVertices[1].x = pfVertices[0];
		pVertices[1].y = pfVertices[1];
		pVertices[1].z = pfVertices[2];

		//�|���S���̒��_�O�ڂ��擾.
		pfVertices = reinterpret_cast<FLOAT*>
			(&pbVertices[dwStride * pwPoly[dwPolyIndex * 3 + 2]]);

		pVertices[2].x = pfVertices[0];
		pVertices[2].y = pfVertices[1];
		pVertices[2].z = pfVertices[2];

		pMesh->UnlockIndexBuffer();	//���b�N����.
		VB->Unlock();				//���b�N����.
	}
	VB->Release();	//�s�v�ɂȂ�����ŉ��.


	return S_OK;
}

//��]�l����(1���ȏサ�Ă���Ƃ��̒���).
void CStaticMeshObject::ClampDirection(float* dir)
{
	if (*dir > D3DX_PI * 2.0f)
	{
		//����ȏサ�Ă���/
		*dir -= D3DX_PI * 2.0f; //2��(360��)������.
	}

	if (*dir > D3DX_PI * 2.0f)
	{
		//�ċA�֐�.
		ClampDirection(dir);
	}
}
