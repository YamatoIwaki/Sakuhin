#pragma once
#pragma once
#include "CKaitoHpGauge.h"

/********************************************
*		ƒJƒCƒg‚Ì”wŒi‘Ì—ÍƒQ[ƒW
**/

class CKaitoBackGauge
	: public CKaitoHpGauge
	, public SingletonManager<CKaitoBackGauge>
{
private:
	CKaitoBackGauge();
	~CKaitoBackGauge() override;
public:
	friend SingletonManager<CKaitoBackGauge>;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw();


protected:

};

