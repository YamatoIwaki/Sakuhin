#pragma once
#include "CBossHpGauge.h"

/********************************************
*		�{�X�̌��݂̗̑̓Q�[�W
**/

class CBossHpCurrentGauge
	: public CBossHpGauge
	, public SingletonManager<CBossHpCurrentGauge>
{
private:
	CBossHpCurrentGauge();
	~CBossHpCurrentGauge() override;
public:
	friend SingletonManager<CBossHpCurrentGauge>;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw();


protected:

};

