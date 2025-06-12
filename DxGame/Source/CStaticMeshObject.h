#pragma once

#include "CGameObject.h"
#include "CStaticMesh.h"
#include "CRay.h"

/************************************************************
*	�X�^�e�B�b�N���b�V���I�u�W�F�N�g�N���X.
**/
class CStaticMeshObject
	: public CGameObject
{
public:
	CStaticMeshObject();
	virtual ~CStaticMeshObject() override;

	//CGameObject�ŏ������z�֐��̐錾������Ă�̂ł�����Œ�`������.
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw( D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera ) override;

	void DrawShadowMap();

	//���b�V����ڑ�����.
	void AttachMesh( CStaticMesh& pMesh ){
		m_pMesh = &pMesh;
	}
	//���b�V����؂藣��.
	void DetachMesh(){
		m_pMesh = nullptr;
	}

	//���C�ƃ��b�V���̓����蔻��.
	bool IsHitForRay(
		const RAY& pRay,			//���C�\����.
		float* pfDistance,			//(out)����
		D3DXVECTOR3* pvIntersect,	//(out)�����_
		D3DXVECTOR3* pNormal=nullptr);	//(out)�@��(�x�N�g��).	
	//�ǂ���̈ʒu���v�Z����.
	void CalculatePositionFromWall(CROSSRAY* pCrossRay);

private:
	//�����ʒu�̃|���S���̒��_��������.
	HRESULT FindVerticesOnPoly(
		LPD3DXMESH pMesh,
		DWORD dwPolyIndex,
		D3DXVECTOR3* pVertices);	//(out)���_���.

	//��]�l����(1���ڏ����Ă���Ƃ��̒���).
	void ClampDirection(float* dir);	//�ċA�֐�.
protected:
	CStaticMesh*		m_pMesh;
};
