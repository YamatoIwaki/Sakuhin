#include "CSprite3DManager.h"

CSprite3DManager::CSprite3DManager()
	: m_pSprite3D	()
{
	//�C���X�^���X����.
	for (int i = 0; i < enList::max; i++)
	{
		m_pSprite3D[i] = new CSprite3D();
	}
}

CSprite3DManager::~CSprite3DManager()
{
	//�C���X�^���X�j��.
	for (int i = enList::max - 1; i >= 0; i--)
	{
		SAFE_DELETE(m_pSprite3D[i]);
	}
}

bool CSprite3DManager::Load()
{
	//Hp�X�v���C�g�̍\����.
	CSprite3D::SPRITE_STATE SS_Hp = { 1.0f, 0.3f, 500.0f, 375.0f, 500.0f, 125.0f };

	struct SoundList
	{
		int listNo;				//enList�񋓌^��ݒ�.
		const TCHAR path[256];	//�t�@�C���̖��O(�p�X�t��).
		CSprite3D::SPRITE_STATE ss;	//�X�v���C�g�X�e�[�g
	};
	SoundList SList[] =
	{
		{ enList::Hp,	_T("Data\\Texture\\Hp.png"), SS_Hp },



	};
	//�z��̍ő�v�f�����Z�o (�z��S�̂̃T�C�Y/�z��1���̃T�C�Y).
	int list_max = sizeof(SList) / sizeof(SList[0]);
	for (int i = 0; i < list_max; i++)
	{
		if (m_pSprite3D[SList[i].listNo]->Init(SList[i].path, SList[i].ss))
		{
			return false;
		}
	}

	return true;
}
