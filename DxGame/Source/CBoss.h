#pragma once
#include "CEnemy.h"

/*********************************************
*				�{�X�N���X
**/



class CBoss
	: public CEnemy
	, public SingletonManager<CBoss>
{
public:


	static constexpr float	MaxHp					= 800.0f;	//�ő�̗�
	static constexpr float	MoveSpeed				= 0.01f;	//�ړ��X�s�[�h
	static constexpr float  SordAttackMoveSpeed		= 0.04f;	//���U�����̈ړ��X�s�[�h
	static constexpr int	StepCoolTime			= 150;		//�T�C�h�A���ɃX�e�b�v���鎞��
	static constexpr float	StepSpeed				= 0.1f;		//�X���C�h�X�s�[�h
	static constexpr int	MaxStepTime				= 65;		//�}�b�N�X�X���C�h����

	static constexpr double AnimSpeed				= 0.006;	//�A�j���[�V�����X�s�[�h

	static constexpr int	AttackCoolTime			= 180;		//�U�����鎞��
	static constexpr float	MaxAttackLength			= 8.0f;		//�U�����[�V�����̋����̍ő�̒���

	static constexpr float	RadiusOfSwordAttack		= 1.0f;		//�ڕW����̔��a(���U���̏ꍇ)
	static constexpr float	RadiusOfTargetDefalt	= 8.0f;		//�ڕW����̔��a

	static constexpr float  EyePos					= 3.8f;		//�ڂ̈ʒl

private:
	CBoss();
	~CBoss() override;
public:
	friend class SingletonManager<CBoss>;
	
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void DrawShadowMap() override;

	void RayUpdate() override;

	bool GetMissile() { return m_IsMissile; }

	void SetIsMissile(bool isMissile) { m_IsMissile = isMissile; }

	bool GetIsSAttack() { return m_IsSAttack; }

	void SetIsSAttack(bool isSAttack) { m_IsSAttack = isSAttack; }

public:

	enum enBossMotion
	{
		B_M_Boot,			//�N��
		B_M_Wait,			//�ҋ@
		B_M_Straight,		//�O�ړ�
		B_M_RightStep,		//�E�X�e�b�v
		B_M_LeftStep,		//���X�e�b�v
		B_M_BackStep,		//�o�b�N�X�e�b�v
		B_M_SwordAttack,	//�ߐڍU��
		B_M_Missile,		//�~�T�C���U��
		B_M_Die,			//����ł���


	};

	enum enBossAnimation
	{
		B_A_Boot,			//�N��
		B_A_Wait,			//�ҋ@
		B_A_Wince,			//����
		B_A_SwordAttack,	//�ߐڍU��
		B_A_Missile,		//�~�T�C���U��
		B_A_BeginGuard,		//�K�[�h�o���͂���
		B_A_Guard,			//�K�[�h��
		B_A_BeginStraight,	//�O�ɐi�݂���
		B_A_Straight,		//�O�ɐi��ł���
		B_A_EndStraight,	//�~�܂鎞
		B_A_BeginRightStep,	//�E�ɐi�݂���
		B_A_RightStep,		//�E�ɐi��ł���
		B_A_EndRightStep,	//�~�܂鎞
		B_A_BeginLeftStep,	//���ɐi�݂���
		B_A_LeftStep,		//���ɐi��ł���
		B_A_EndLeftStep,	//�~�܂鎞
		B_A_BeginBackStep,	//��뉺����
		B_A_BackStep,		//���ɉ������Ă���
		B_A_EndBackStep,	//�~�܂鎞
		B_A_BeginLose,		//��������
		B_A_Lose,			//�����|�[�Y
		B_A_BeginWin,		//��������
		B_A_Win,			//�����|�[�Y
	};

private:
	void ChangeMotion(int Motion = 1, int Animation = 1, double ASpeed = AnimSpeed);

	//�X�e�b�v�̕��������߂�
	void WhatStep();

	//�U�����[�V���������߂�
	void WhatAttack();

	std::vector<double> CulculateWeightsFromDistance(float distance);

protected:

	bool		m_IsApper;		//�o�����Ă��邩�ǂ���

	int			m_StepCoolTime;	//�X���C�h�N�[���^�C������
	int			m_StepTime;		//�X���C�h������

	bool		m_IsMissile;	//���˂������ǂ���
	bool		m_IsSetMissile;	//�Z�b�g�����ǂ���

	bool		m_IsSAttack;	//���U���̓����蔻�肪���邩�ǂ���
	bool		m_ChangeSAttack;//�����蔻�肪��x�؂�ւ�������ǂ���

};

