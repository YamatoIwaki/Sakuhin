#pragma once
#include "CStaticMeshObject.h"


/*******************************************
*		�U�R�̒e
**/


class CZakoBullet
	: public CStaticMeshObject
{
public:
	static constexpr int MaxAppeaTime = 500;	//�e�̏o������
	static constexpr float BulletSpeed = 0.5f;	//�e�̑��x
	static constexpr float BulletDamage = 5.0f;	//�e�_���[�W

public:
	CZakoBullet();
	virtual ~CZakoBullet() override;

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	bool IsShotting() { return m_IsShot; }

	void SetShotting(bool isShot) { m_IsShot = isShot; }

private:
	bool		m_IsShot;

	int			m_AppeaTime;


};

