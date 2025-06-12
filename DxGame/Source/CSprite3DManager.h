#pragma once
#include "CSprite3D.h"
#include "CSingletonManager.h"

/*********************************************************
*			�X�v���C�g3D�}�l�[�W���[
**/




class CSprite3DManager
	: public SingletonManager<CSprite3DManager>
{
public:
	enum enList
	{
		Hp,		//Hp

		//�ǉ��������ɒǉ�
		max,
	};


private:
	CSprite3DManager();
public:
	friend class SingletonManager<CSprite3DManager>;

	~CSprite3DManager();

	//�f�[�^�Ǎ��֐�.
	bool Load();


	static CSprite3D* GetSprite(enList list) { return CSprite3DManager::GetInstance()->m_pSprite3D[list]; }


private:

	CSprite3D* m_pSprite3D[enList::max];

};

