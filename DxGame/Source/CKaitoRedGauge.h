#pragma once
#include "CKaitoHpGauge.h"

/********************************************
*		カイトの赤体力ゲージ
**/

class CKaitoRedGauge
	: public CKaitoHpGauge
	, public SingletonManager<CKaitoRedGauge>
{
public:
	static constexpr float ReduceGauge = 0.001f;
	static constexpr int LateTime = 30;
private:
	CKaitoRedGauge();
	~CKaitoRedGauge() override;
public:
	friend SingletonManager<CKaitoRedGauge>;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw();


protected:
	int		m_LateTime;

};

