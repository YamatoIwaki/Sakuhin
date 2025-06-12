#include "CSceneManager.h"

CSceneManager::CSceneManager()
	: m_Scene		()
	, m_NextScene	(nullptr)
	, m_IsChangeing	(false)
{
}

CSceneManager::~CSceneManager()
{

	//全てのシーンを終了してから削除.
	while (!m_Scene.empty())
	{
		m_Scene.pop();			//シーンの削除
	}

}


void CSceneManager::Update()
{
	//fade更新
	CFadeBlack::GetInstance()->Update();

	//フェードアウト完了してからシーン切り替え
	if (m_IsChangeing && !CFadeBlack::GetInstance()->IsFading() && m_NextScene)
	{
		if (!m_Scene.empty())
		{
			m_Scene.top()->Exit();
			m_Scene.pop();
		}

		m_Scene.push(std::move(m_NextScene));
		m_Scene.top()->Init();

		CFadeBlack::GetInstance()->StartFadeIn(1.0f);	//fadeイン開始
		m_IsChangeing = false;
	}



	if (!m_Scene.empty())
	{
		m_Scene.top()->Update();	//現在のシーンをアップデート
	}
}

void CSceneManager::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if (!m_Scene.empty())
	{
		m_Scene.top()->Draw(View, Proj, Light, Camera);		//現在のシーンを描画
	}

	CFadeBlack::GetInstance()->Draw();


}

CSceneBase* CSceneManager::GetCurrentScene()
{
	if (!m_Scene.empty())
	{
		return m_Scene.top().get();
	}
	return nullptr;
}

void CSceneManager::PushScene(std::unique_ptr<CSceneBase> newScene)
{
	if (!m_Scene.empty())
	{
		m_Scene.top()->Exit();	//現在のシーンの終了処理
	}

	m_Scene.push(std::move(newScene));
	m_Scene.top()->Init();	//新しいシーンを初期化
}

void CSceneManager::PopScene()
{
	if (!m_Scene.empty())
	{
		m_Scene.top()->Exit();	//現在のシーンの終了処理
		m_Scene.pop();			//現在のシーンを削除
		if (!m_Scene.empty())
		{
			m_Scene.top()->Init();	//次のシーンを初期化
		}
	}
}

void CSceneManager::ChangeScene(std::unique_ptr<CSceneBase> newScene)
{
	if (!m_Scene.empty())
	{

		m_Scene.top()->Exit();	//現在のシーンを終了
		m_Scene.pop();			//現在のシーンを削除
	}

	m_Scene.push(std::move(newScene));	//新しいシーンをスタックする
	m_Scene.top()->Init();		//新しいシーンを初期化

}

void CSceneManager::ChangeSceneWithFade(std::unique_ptr<CSceneBase> newScene)
{
	if (!m_IsChangeing && !CFadeBlack::GetInstance()->IsFading())
	{
		m_NextScene = std::move(newScene);
		CFadeBlack::GetInstance()->StartFadeOut(2.0f);
		m_IsChangeing = true;
	}

}

