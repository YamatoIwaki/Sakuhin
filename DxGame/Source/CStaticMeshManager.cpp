#include "CStaticMeshManager.h"

CStaticMeshManager::CStaticMeshManager()
	: m_pStaticMesh		()
{
	//�C���X�^���X����.
	for (int i = 0; i < enList::max; i++)
	{
		m_pStaticMesh[i] = new CStaticMesh();
	}
}

CStaticMeshManager::~CStaticMeshManager()
{
	//�C���X�^���X�j��.
	for (int i = enList::max - 1; i >= 0; i--)
	{
		SAFE_DELETE(m_pStaticMesh[i]);
	}
}

bool CStaticMeshManager::Load()
{

	struct SoundList
	{
		int listNo;				//enList�񋓌^��ݒ�.
		const TCHAR path[256];	//�t�@�C���̖��O(�p�X�t��).
	};
	SoundList SList[] =
	{
		{ enList::ZakoBullet,			_T("Data\\Mesh\\Static\\EnemyBullet\\EnemyBullet.x") },
		{ enList::Missile,				_T("Data\\Mesh\\Static\\Missile\\Missile.x") },
		{ enList::Stage,				_T("Data\\Mesh\\Static\\Stage\\Stage1.x") },
		{ enList::HitPoint,				_T("Data\\Mesh\\Static\\Bullet\\bullet.x") },
		{ enList::RangeOfStage,			_T("Data\\Mesh\\Static\\Stage\\RangeOfStage.x") },
		{ enList::ApperRangeOfStage,	_T("Data\\Mesh\\Static\\Stage\\AppearRangeOfStage.x") },



	};
	//�z��̍ő�v�f�����Z�o (�z��S�̂̃T�C�Y/�z��1���̃T�C�Y).
	int list_max = sizeof(SList) / sizeof(SList[0]);
	for (int i = 0; i < list_max; i++)
	{
		if (m_pStaticMesh[SList[i].listNo]->Init(SList[i].path))
		{
			return false;
		}
	}

	return true;
}
