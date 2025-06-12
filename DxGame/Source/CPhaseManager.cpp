#include "CPhaseManager.h"

CPhaseManager::CPhaseManager()
	: m_Phase()
{
}

CPhaseManager::~CPhaseManager()
{

	//全てのフェーズを終了してから削除.
	while (!m_Phase.empty())
	{
		m_Phase.top()->Exit();	//フェーズ終了処理
		m_Phase.pop();			//フェーズの削除
	}

}


void CPhaseManager::Update()
{
	if (!m_Phase.empty())
	{
		m_Phase.top()->Update();	//現在のフェーズをアップデート
	}
}

void CPhaseManager::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if (!m_Phase.empty())
	{
		m_Phase.top()->Draw(View, Proj, Light, Camera);		//現在のフェーズを描画
	}

}

void CPhaseManager::DrawShadow()
{
	if (!m_Phase.empty())
	{
		m_Phase.top()->DrawShadow();		//現在のフェーズを描画
	}
}

CPhaseBase* CPhaseManager::GetCurrentPhase()
{
	if (!m_Phase.empty())
	{
		return m_Phase.top().get();
	}
	return nullptr;
}

void CPhaseManager::PushPhase(std::unique_ptr<CPhaseBase> newPhase)
{
	if (!m_Phase.empty())
	{
		m_Phase.top()->Exit();	//現在のフェーズの終了処理
	}

	m_Phase.push(std::move(newPhase));
	m_Phase.top()->Init();	//新しいフェーズを初期化
}

void CPhaseManager::PopPhase()
{
	if (!m_Phase.empty())
	{
		m_Phase.top()->Exit();	//現在のフェーズの終了処理
		m_Phase.pop();			//現在のフェーズを削除
		if (!m_Phase.empty())
		{
			m_Phase.top()->Init();	//次のフェーズを初期化
		}
	}
}

void CPhaseManager::ChangePhase(std::unique_ptr<CPhaseBase> newPhase)
{
	if (!m_Phase.empty())
	{
		m_Phase.top()->Exit();	//現在のフェーズを終了
		m_Phase.pop();			//現在のフェーズを削除
	}

	m_Phase.push(std::move(newPhase));	//新しいフェーズをスタックする
	m_Phase.top()->Init();		//新しいフェーズを初期化

}

