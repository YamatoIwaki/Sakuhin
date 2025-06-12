#pragma once

#include <stack>
#include <memory>

#include "CSceneBase.h"
#include "CSingletonManager.h"

class CSceneManager
	: public SingletonManager<CSceneManager>
{
public:
	friend class SingletonManager<CSceneManager>;

	void Update();

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera);

	//現在のシーンを取得
	CSceneBase* GetCurrentScene();

	//シーンを追加する
	void PushScene(std::unique_ptr<CSceneBase> newScene);

	//シーンの削除
	void PopScene();

	//シーンを切り替える
	void ChangeScene(std::unique_ptr<CSceneBase> newScene);

	//シーンを切り替える
	void ChangeSceneWithFade(std::unique_ptr<CSceneBase> newScene);

protected:
	CSceneManager();
	~CSceneManager();

	std::stack<std::unique_ptr<CSceneBase>> m_Scene;
	std::unique_ptr<CSceneBase> m_NextScene = nullptr;

	bool m_IsChangeing = false;
};

