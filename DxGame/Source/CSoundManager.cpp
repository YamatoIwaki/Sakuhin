#include "CSoundManager.h"
#include "MyMacro.h"

CSoundManager::CSoundManager()
	: m_pSound	()
{
	//インスタンス生成.
	for( int i = 0; i < enList::max; i++ )
	{
		m_pSound[i] = new CSound();
	}
}

CSoundManager::~CSoundManager()
{
	Release();

	//インスタンス破棄.
	for( int i = enList::max - 1; i >= 0; i-- )
	{
		SAFE_DELETE( m_pSound[i] );
	}
}

//サウンドデータ読込関数.
bool CSoundManager::Load( HWND hWnd )
{
	struct SoundList
	{
		int listNo;				//enList列挙型を設定.
		const TCHAR path[256];	//ファイルの名前(パス付き).
		const TCHAR alias[32];	//エイリアス名.
		int	volume = 100;		//音量設定
	};
	SoundList SList[] =
	{
		{ enList::BGM_Title,		_T("Data\\Sound\\BGM\\Title.mp3"),			_T("BGM_Title")			 },
		{ enList::BGM_GameMain,		_T("Data\\Sound\\BGM\\Fight.mp3"),			_T("BGM_GameMain")		 },
		{ enList::BGM_Boss,			_T("Data\\Sound\\BGM\\Boss.mp3"),			_T("BGM_Boss")			 },
		{ enList::BGM_GameOver,		_T("Data\\Sound\\BGM\\GameOver.mp3"),		_T("BGM_GameOver")		 },
		{ enList::BGM_GameClear,	_T("Data\\Sound\\BGM\\GameClear.mp3"),		_T("BGM_GameClear")		 },
																										 
																										 
																										 
		{ enList::SE_RoboDie,		_T("Data\\Sound\\SE\\RobotDie.mp3"),		_T("SE_RoboDie")	, 300},
		{ enList::SE_P_Sword1,		_T("Data\\Sound\\SE\\Sword1.mp3"),			_T("SE_P_Sword1")		 },
		{ enList::SE_P_Sword2,		_T("Data\\Sound\\SE\\Sword2.mp3"),			_T("SE_P_Sword2")		 },
		{ enList::SE_P_Sword3,		_T("Data\\Sound\\SE\\Sword3.mp3"),			_T("SE_P_Sword3")		 },
		{ enList::SE_P_Damage,		_T("Data\\Sound\\SE\\PlayerDamage.mp3"),	_T("SE_P_Damage")	, 300},
		{ enList::SE_P_Die,			_T("Data\\Sound\\SE\\PlayerDie.mp3"),		_T("SE_P_Die")			 },

		{ enList::SE_B_Die,			_T("Data\\Sound\\SE\\BossDie.mp3"),			_T("SE_B_Die")		, 300},
		{ enList::SE_B_Sride,		_T("Data\\Sound\\SE\\BossSride.mp3"),		_T("SE_B_Sride")		 },
		{ enList::SE_B_Sword,		_T("Data\\Sound\\SE\\BossSword.mp3"),		_T("SE_B_Sword")		 },
		{ enList::SE_B_Missile,		_T("Data\\Sound\\SE\\BossMissile.mp3"),		_T("SE_B_Missile")		 },

		{ enList::SE_Z_Shot,		_T("Data\\Sound\\SE\\ZakoShot.mp3"),		_T("SE_Z_Shot")			 },
		{ enList::SE_Z_Tacle,		_T("Data\\Sound\\SE\\ZakoTackle.mp3"),		_T("SE_Z_Tacle")		 },
		{ enList::SE_Z2_Attack,		_T("Data\\Sound\\SE\\Zako2Attack.mp3"),		_T("SE_Z2_Attack")		 },
		{ enList::SE_Z2_Sride,		_T("Data\\Sound\\SE\\Zako2Sride.mp3"),		_T("SE_Z2_Sride")		 },

		{ enList::SE_Click,			_T("Data\\Sound\\SE\\Click.mp3"),			_T("SE_Click")		 },
		{ enList::SE_PhaseStart,	_T("Data\\Sound\\SE\\Start.mp3"),			_T("SE_PhaseStart")		 },

	};
	//配列の最大要素数を算出 (配列全体のサイズ/配列1つ分のサイズ).
	int list_max = sizeof( SList ) / sizeof( SList[0] );
	for( int i = 0; i < list_max; i++ )
	{
		if( m_pSound[SList[i].listNo]->Open(
			SList[i].path,
			SList[i].alias,
			hWnd,
			SList[i].volume) == false)
		{
			return false;
		}
	}

	return true;
}

//サウンドデータ解放関数.
void CSoundManager::Release()
{
	//開いた時と逆順で閉じる.
	for( int i = enList::max - 1; i >= 0; i-- )
	{
		if( m_pSound[i] != nullptr )
		{
			m_pSound[i]->Close();
		}
	}
}
