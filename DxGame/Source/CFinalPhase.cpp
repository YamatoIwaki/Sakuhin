#include "CFinalPhase.h"

#include "CEffect.h"		//Effekseerを使うためのクラス.

CFinalPhase::CFinalPhase()
{
	m_pPhaseText = std::make_unique<CPhaseTextBase>();

	m_pPhaseText->AttachSprite(*CSprite2DManager::GetSprite(CSprite2DManager::FinalPhaseText));

	//ここのテキストだけ大きさが違うので正しくセットする
	m_pPhaseText->SetPosition(D3DXVECTOR3(WND_W / 2 - 300.0f, WND_H / 2 - 150.0f, 0.0f));
}

CFinalPhase::~CFinalPhase()
{
}

void CFinalPhase::Init()
{
	CStartText::GetInstance()->Init();

	CBoss::GetInstance()->Init();

	static ::EsHandle hEffect = -1;

	hEffect = CEffect::Play(CEffect::ApperBoss, CBoss::GetInstance()->GetPosition());
	


}

void CFinalPhase::Update()
{
	CSoundManager::PlayLoop(CSoundManager::BGM_Boss);

	CBoss::GetInstance()->RayUpdate();

	CGround::GetInstance()->Update();
	
	CBoss::GetInstance()->Update();

	CBossMissileSet::GetInstance()->Update();

	CBossSAHitPos::GetInstance()->Update();


	CBossHpCurrentGauge::GetInstance()->Update();
	CBossHpLateGauge::GetInstance()->Update();


	m_pPhaseText->Update();

	//テキストが消えるまで動かないようにする
	if (!m_pPhaseText->IsDrawEnd()) { return; }

	CStartText::GetInstance()->Update();



	for (int a = 0; a <= CKaito::GetInstance()->P_C_Attack3; a++) {
		if (CCapsuleCollision::IsHit(CKaito::GetInstance()->GetAttackColl(a), CBoss::GetInstance()->GetCapsule()))
		{
			//カイト攻撃時の当たり判定処理
			CKaito::GetInstance()->GetAttackColl(a).SetHitJudg(false);
			CBoss::GetInstance()->Damage(CKaito::SwordDamage);
		}
	}

}

void CFinalPhase::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CBoss::GetInstance()->Draw(View,Proj,Light,Camera);

	CBossMissileSet::GetInstance()->Draw(View, Proj, Light, Camera);

	//ボスHpゲージの描画
	CDirectX11::GetInstance()->SetDepth(false);
	CBossHpBackGauge::GetInstance()->Draw();
	CBossHpLateGauge::GetInstance()->Draw();
	CBossHpCurrentGauge::GetInstance()->Draw();
	CBossHpText::GetInstance()->Draw();

	if (m_pPhaseText->IsDrawEnd())
	{
		CConcentLine::GetInstance()->Draw();

		CStartText::GetInstance()->Draw();
	}

	m_pPhaseText->Draw();
	CDirectX11::GetInstance()->SetDepth(true);


}

void CFinalPhase::DrawShadow()
{

	CBoss::GetInstance()->DrawShadowMap();

}

void CFinalPhase::Exit()
{
}
