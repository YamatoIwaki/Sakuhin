#pragma once

#include "CGameObject.h"
#include "CSkinMesh.h"
#include "CRay.h"

/************************************************************
*	�X�L�����b�V���I�u�W�F�N�g�N���X.
**/
class CSkinMeshObject
	: public CGameObject
{
public:
	CSkinMeshObject();
	virtual ~CSkinMeshObject() override;

	//CGameObject�ŏ������z�֐��̐錾������Ă�̂ł�����Œ�`������.
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	virtual void DrawShadowMap();

	virtual void QuaterDraw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera);

	//���b�V����ڑ�����.
	void AttachMesh(CSkinMesh& pMesh);

	//���b�V����؂藣��.
	void DetachMesh();


	//���C�ƃ��b�V���̓����蔻��.
	bool IsHitForRay(
		const RAY& pRay,			//���C�\����.
		float* pfDistance,			//(out)����
		D3DXVECTOR3* pvIntersect,	//(out)�����_
		D3DXVECTOR3* pNormal=nullptr);	//(out)�@��(�x�N�g��).	
	//�ǂ���̈ʒu���v�Z����.
	void CalculatePositionFromWall(CROSSRAY* pCrossRay);

public:
	//Y�������֐L�΂������C���擾.
	RAY GetRayY() const { return *m_pRayY; }
	//�\�����C���擾.
	CROSSRAY GetCrossRay() const { return *m_pCrossRay; }

	void SetCamPos(const D3DXVECTOR3& pos) { m_CamPos = pos; }

	float GetHp() { return m_Hp; }

	void Damage(float D) { m_Hp -= D; }


private:
	//�����ʒu�̃|���S���̒��_��������.
	HRESULT FindVerticesOnPoly(
		LPD3DXMESH pMesh,
		DWORD dwPolyIndex,
		D3DXVECTOR3* pVertices);	//(out)���_���.

	//��]�l����(1���ڏ����Ă���Ƃ��̒���).
	void ClampDirection(float* dir);	//�ċA�֐�.



protected:
	CSkinMesh* m_pMesh;
	
	//�A�j���[�V����
	LPD3DXANIMATIONCONTROLLER	m_pAnimCtrl;	//�A�j���[�V�����R���g���[��.
	int							m_AnimNo;		//�G���F�A�j���[�V����.
	double						m_AnimTime;		//�G���F�A�j���[�V�����o�ߎ���.
	double						m_AnimSpeed;	//�A�j���[�V�������x.
	D3DXVECTOR3					m_BonePos;		//�G���F�{�[�����W.


	RAY*			m_pRayY;					//Y�����֐L�΂������C.
	CROSSRAY*		m_pCrossRay;				//�\�����C

	D3DXVECTOR3		m_CamPos;

	D3DXMATRIX		m_mRotation;				//���[���h�s����擾

	D3DXVECTOR3		m_QuaterRot;	

	float			m_Hp;
};
