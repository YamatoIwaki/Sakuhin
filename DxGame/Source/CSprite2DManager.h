#pragma once
#include "CSprite2D.h"
#include "CSingletonManager.h"

/*********************************************************
*			�X�v���C�g2D�}�l�[�W���[
**/




class CSprite2DManager
	: public SingletonManager<CSprite2DManager>
{
public:
	enum enList
	{
		KaitoHp,		//�J�C�gHp
		BossHp,			//�{�XHp
		FadeBlack,		//fade�p�̍��w�i
		OpeInstruct,	//��������p�摜
		OpeInstructSub,	//��������p�摜(�T�u)

		KaitoHpText,	//�J�C�gHp�e�L�X�g
		BossHpText,		//�{�XHp�e�L�X�g
		ClickText,		//Click�e�L�X�g
		TitleText,		//�^�C�g���e�L�X�g
		GameClearText,	//�Q�[���N���A�e�L�X�g
		GameOverText,	//�Q�[���I�[�o�[�e�L�X�g
		Phase1Text,		//�t�F�[�Y�P
		Phase2Text,		//�t�F�[�Y�Q
		FinalPhaseText,	//�t�@�C�i���t�F�[�Y
		StartText,		//�X�^�[�g�e�L�X�g
		LoadingText,	//
		//�ǉ��������ɒǉ�
		max,
	};


private:
	CSprite2DManager();
public:
	friend class SingletonManager<CSprite2DManager>;

	~CSprite2DManager();

	//�f�[�^�Ǎ��֐�.
	bool Load();


	static CSprite2D* GetSprite(enList list) { return CSprite2DManager::GetInstance()->m_pSprite2D[list]; }


private:

	CSprite2D* m_pSprite2D[enList::max];

};

