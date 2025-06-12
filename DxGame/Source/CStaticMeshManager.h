#pragma once
#include "CStaticMesh.h"
#include "CSingletonManager.h"

/*********************************************************
*			�X�^�e�B�b�N���b�V���}�l�[�W���[
**/




class CStaticMeshManager
	: public SingletonManager<CStaticMeshManager>
{
public:
	enum enList
	{
		ZakoBullet,		//�U�R�̒e
		Missile,		//�~�T�C��
		Stage,			//�X�e�[�W
		HitPoint,		//�����蔻��̈ʒu
		RangeOfStage,	//�X�e�[�W�̈ړ��͈�
		ApperRangeOfStage,	//�X�e�[�W�̈ړ��͈�(���o��)

		//�ǉ��������ɒǉ�
		max,
	};


private:
	CStaticMeshManager();
public:
	friend class SingletonManager<CStaticMeshManager>;

	~CStaticMeshManager();

	//�f�[�^�Ǎ��֐�.
	bool Load();


	static CStaticMesh* GetMesh(enList list) { return CStaticMeshManager::GetInstance()->m_pStaticMesh[list]; }


private:

	CStaticMesh* m_pStaticMesh[enList::max];

};

