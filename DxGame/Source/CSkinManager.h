#pragma once
#include "CSkinMesh.h"
#include "CSingletonManager.h"

/*********************************************************
*			�X�^�e�B�b�N���b�V���}�l�[�W���[
**/




class CSkinManager
	: public SingletonManager<CSkinManager>
{
public:
	enum enList
	{
		Kaito,		//�J�C�g
		Zako,		//�U�R
		Zako2,		//�U�R�Q
		Boss,		//�{�X


		//�ǉ��������ɒǉ�
		max,
	};


private:
	CSkinManager();
public:
	friend class SingletonManager<CSkinManager>;

	~CSkinManager();

	//�f�[�^�Ǎ��֐�.
	bool Load();


	static CSkinMesh* GetMesh(enList list) { return CSkinManager::GetInstance()->m_pSkinMesh[list]; }


private:

	CSkinMesh* m_pSkinMesh[enList::max];

};

