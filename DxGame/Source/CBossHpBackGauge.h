#pragma once
#include "CBossHpGauge.h"

/********************************************
*		ƒ{ƒX‚Ì”wŒi‘Ì—ÍƒQ[ƒW
**/

class CBossHpBackGauge
	: public CBossHpGauge
	, public SingletonManager<CBossHpBackGauge>
{
private:
	CBossHpBackGauge();
	~CBossHpBackGauge() override;
public:
	friend SingletonManager<CBossHpBackGauge>;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw();


protected:

};

