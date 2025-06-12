#pragma once

#include <stack>
#include <memory>

#include "CPhaseBase.h"
#include "CSingletonManager.h"

class CPhaseManager
	: public SingletonManager<CPhaseManager>
{
public:
	friend class SingletonManager<CPhaseManager>;

	void Update();

	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera);

	void DrawShadow();


	//現在のシーンを取得
	CPhaseBase* GetCurrentPhase();

	//シーンを追加する
	void PushPhase(std::unique_ptr<CPhaseBase> newPhase);

	//シーンの削除
	void PopPhase();

	//シーンを切り替える
	void ChangePhase(std::unique_ptr<CPhaseBase> newPhase);

protected:
	CPhaseManager();
	~CPhaseManager();

	std::stack<std::unique_ptr<CPhaseBase>> m_Phase;


};

