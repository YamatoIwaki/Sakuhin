#pragma once
#include "CSound.h"		//�T�E���h�N���X.

/**************************************************
*	�T�E���h�}�l�[�W���[�N���X.
*	Manager(�}�l�[�W���[)�F�Ǘ���.
*		Singleton(�V���O���g���F�f�U�C���p�^�[���̂P��)�ō쐬.
**/
class CSoundManager
{
public:
	//�T�E���h���X�g�񋓌^.
	enum enList
	{
		BGM_Title,		//�^�C�g��
		BGM_GameMain,	//�Q�[�����C��
		BGM_Boss,		//�{�X��
		BGM_GameOver,	//�Q�[���I�[�o�[
		BGM_GameClear,	//�Q�[���N���A


		SE_RoboDie,		//���{�b�g�����񂾂Ƃ�
		SE_P_Sword1,	//�v���C���[�؂�t���U��1
		SE_P_Sword2,	//�v���C���[�؂�t���U��2
		SE_P_Sword3,	//�v���C���[�؂�t���U��3
		SE_P_Damage,	//�v���C���[�U�����󂯂��Ƃ�
		SE_P_Die,		//�v���C���[���񂾂Ƃ�
		SE_B_Die,		//�{�X�����񂾂Ƃ�
		SE_B_Sride,		//�{�X�X���C�h��
		SE_B_Sword,		//�{�X�X�؂�t���U����
		SE_B_Missile,		//�{�X�X�~�T�C����

		SE_Z_Shot,		//�U�R�V���b�g��
		SE_Z_Tacle,		//�U�R�^�b�N����
		SE_Z2_Attack,	//�U�R�Q�U����
		SE_Z2_Sride,	//�U�R�Q�X���C�h��

		SE_Click,		//�N���b�N
		SE_PhaseStart,	//�t�F�[�Y�n�܂鉹

		//������������u�����v�ɒǉ����Ă�������.
		max,		//�ő吔.
	};

public:
	//�C���X�^���X�擾(�B��̃A�N�Z�X�o�H).
	//���֐��̑O��static��t���邱�ƂŃC���X�^���X�������Ȃ��Ă��g�p�ł���.
	static CSoundManager* GetInstance()
	{
		//�B��̃C���X�^���X���쐬����.
		//��static�ō쐬���ꂽ�̂�2��ڈȍ~�́A����1�s�͖��������.
		static CSoundManager s_Instance;	//s_:static�̈Ӗ�.
		return &s_Instance;
	}


	~CSoundManager();

	//�T�E���h�f�[�^�Ǎ��֐�.
	bool Load( HWND hWnd );
	//�T�E���h�f�[�^����֐�.
	void Release();

	//SE���Đ�����.
	static void PlaySE( enList list ) {
		CSoundManager::GetInstance()->m_pSound[list]->PlaySE();
	}
	//���[�v�Đ�����.
	static void PlayLoop( enList list, bool isStart = true) {
		CSoundManager::GetInstance()->m_pSound[list]->PlayLoop(isStart);
	}
	//��~����.
	static void Stop( enList list ) {
		CSoundManager::GetInstance()->m_pSound[list]->Stop();
	}

private://�O������A�N�Z�X�s�\.
	//�O������R���X�g���N�^�ւ̃A�N�Z�X���֎~����.
	CSoundManager();
	//�R�s�[�R���X�g���N�^�ɂ��R�s�[���֎~����.
	//�u=delete�v�Ŋ֐��̒�`���폜�ł���.
	CSoundManager( const CSoundManager& rhs ) = delete;
	//������Z�q�ɂ��R�s�[���֎~����.
	//operator(�I�y���[�^):���Z�q�̃I�[�o�[���[�h�ŁA���Z�̒��g���g���ł���.
	CSoundManager& operator = ( const CSoundManager& rhs ) = delete;

private:
	CSound*		m_pSound[enList::max];
};