#include "CSkinManager.h"

CSkinManager::CSkinManager()
	: m_pSkinMesh()
{
	//�C���X�^���X����.
	for (int i = 0; i < enList::max; i++)
	{
		m_pSkinMesh[i] = new CSkinMesh();
	}
}

CSkinManager::~CSkinManager()
{
	//�C���X�^���X�j��.
	for (int i = enList::max - 1; i >= 0; i--)
	{
		SAFE_DELETE(m_pSkinMesh[i]);
	}
}

bool CSkinManager::Load()
{

	struct SoundList
	{
		int listNo;				//enList�񋓌^��ݒ�.
		const TCHAR path[256];	//�t�@�C���̖��O(�p�X�t��).
	};
	SoundList SList[] =
	{
		{ enList::Kaito,	_T("Data\\Mesh\\Skin\\Player\\kaito\\kaito_s.x") },
		{ enList::Zako,		_T("Data\\Mesh\\Skin\\Zako\\zako.x") },
		{ enList::Zako2,	_T("Data\\Mesh\\Skin\\ZAKO2\\ZAKO2_0.x") },
		{ enList::Boss,		_T("Data\\Mesh\\Skin\\Robot\\tm_f22_robo_anim_Black.X") },

	};
	//�z��̍ő�v�f�����Z�o (�z��S�̂̃T�C�Y/�z��1���̃T�C�Y).
	int list_max = sizeof(SList) / sizeof(SList[0]);
	for (int i = 0; i < list_max; i++)
	{
		if (m_pSkinMesh[SList[i].listNo]->Init(SList[i].path))
		{
			return false;
		}
	}

	return true;
}
