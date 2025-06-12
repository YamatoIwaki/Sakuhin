#pragma once
#include "CBossHpGauge.h"

/********************************************
*		�{�X�̌ォ��x��Ă���̗̓Q�[�W
**/

class CBossHpLateGauge
	: public CBossHpGauge
	, public SingletonManager<CBossHpLateGauge>
{
public:
	static constexpr float ReduceGauge = 0.001f;
	static constexpr int LateTime = 30;
private:
	CBossHpLateGauge();
	~CBossHpLateGauge() override;
public:
	friend SingletonManager<CBossHpLateGauge>;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw();


protected:
	int		m_LateTime;

};

