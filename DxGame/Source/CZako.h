#pragma once
#include "CEnemy.h"

/***********************************************
*		�G���N���X.
**/
class CZako
	: public CEnemy
{

public:

	static constexpr float EyePos				= 1.4f;		//�n�ʂ���ڂ̋���

	static constexpr float RadiusOfTarget		= 10.0f;	//�ڕW����̔��a
	static constexpr float OverRadiusOfTarget	= 8.0f;		//��~��Ԃ��痣�ꂷ������
	static constexpr float MoveSpeed			= 0.004f;	//�ړ��X�s�[�h
	static constexpr float MaxAttackCoolTime	= 100.0f;	//�U���N�[���^�C��

	static constexpr double AnimWaitSpeed		= 0.0002;	//�ҋ@���̃A�j���[�V�����X�s�[�h
	static constexpr double AnimMoveSpeed		= 0.00025;	//�ړ����̃A�j���[�V�����X�s�[�h
	static constexpr double AnimApperSpeed		= 0.0002;	//�o�����̃A�j���[�V�����X�s�[�h

	static constexpr double TackleReadySpeed	= 0.005;	//�ːi�U���̏����������}
	static constexpr double TackleAnimSumSpeed	= 0.00005;	//�ːi�U���������̉��Z�l
	static constexpr float	TackleSpeed			= 0.02f;	//�ːi�U���X�s�[�h
	static constexpr float	MaxTackleTime		= 150.0f;	//�ő�̗�

	static constexpr float	MaxHp				= 60.0f;	//�ő�̗�

	static constexpr int	DieRotSpeed			= 10;		//���񂾂Ƃ��̉�]���x
	static constexpr float	DieSmallSpeed		= 0.00001f;	//���񂾂Ƃ��̏������Ȃ鑬�x
	static constexpr float  DieMinSmallSize		= 0.0001f;	//���񂾂Ƃ��̍ŏ��̏�����

public:
	CZako();
	virtual ~CZako() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void DrawShadowMap() override;

	void RayUpdate() override;

	void SetAnimSpeed(double speed) { m_AnimSpeed = speed; }

	bool GetIsShot() { return m_IsShot; }

	void SetIsShot(bool isShot) { m_IsShot = isShot; }
	
	int	GetMotion() { return m_Motion; }

	bool GetIsTackleMove(){ return m_IsTackleMove; }

public:
	enum enZakoMotion
	{
		Z_M_Stop,		//�~�܂�
		Z_M_Move,		//�ړ�
		Z_M_Shot,		//�V���b�g���[�V����
		Z_M_Tackle,		//�̓�����
		Z_M_Die,		//����
	};

	enum enZakoAnimation
	{
		Z_A_Wait = 0,			//�ҋ@
		Z_A_Move,				//�ړ�
		Z_A_Appear,				//�o��
	};

private:
	void ChangeMotion(int Motion = 0, int Animation = 0, double ASpeed = AnimSpeed);

	//�ːi�U�����̖ڕW�ʒu���Z�o
	void TacklePosSet();

	//�U�����[�V�����I��
	void ChoiceRandomMotion();


protected:

	//�V���b�g�U��
	bool			m_IsShot;


	//�ːi�U��
	D3DXVECTOR3		m_TacklePos;	//�ːi�U���̖ڕW�ʒu
	D3DXVECTOR3		m_TackleLength;	//�ːi�U���̖ڕW�ʒu�Ƃ̒���
	bool			m_IsTackleMove;	//���������ǂ���
	int				m_TackleTime;	//�i���ɖڕW�ʒu�ɂ��Ȃ��ꍇ

};

