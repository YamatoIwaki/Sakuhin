#pragma once
#pragma once
#include "CKaitoHpGauge.h"

/********************************************
*		�J�C�g�̔w�i�̗̓Q�[�W
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

