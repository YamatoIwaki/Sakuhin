#pragma once
#include "CKaitoHpGauge.h"

/********************************************
*		�J�C�g�̗Ἀ̗Q�[�W
**/

class CKaitoGreenGauge
	: public CKaitoHpGauge
	, public SingletonManager<CKaitoGreenGauge>
{
private:
	CKaitoGreenGauge();
	~CKaitoGreenGauge() override;
public:
	friend SingletonManager<CKaitoGreenGauge>;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw();


protected:

};

