#pragma once
#include "CEnemy.h"

/***************************************************
*			�U�R�Q�N���X
**/


class CZako2
	: public CEnemy
{
public:

	static constexpr float			MaxHp				= 100.0f;	//�ő�Hp
	static constexpr double			AnimSpeed			= 0.003;
	static constexpr float			MoveSpeed			= 0.01f;	//�ړ��X�s�[�h
	static constexpr float			SrideSpeed			= 0.1f;		//�X���C�h�X�s�[�h
	static constexpr int			MaxSrideTime		= 65;		//�}�b�N�X�X���C�h����
	static constexpr float			MaxAttackCoolTime	= 100.0f;	//�U���N�[���^�C��

	static constexpr float			RadiusOfTarget		= 0.8f;		//�ڕW����̔��a
	static constexpr float			OverRadiusOfTarget	= 2.0f;		//�ڕW���痣�ꂷ������
	
	static constexpr double			IsAttackHitBegin	= 0.2;		//�U������n�܂�
	static constexpr double			IsAttackHitEnd		= 0.5;		//�U������I���

	static constexpr int			DieRotSpeed			= 10;		//���񂾂Ƃ��̉�]���x
	static constexpr float			DieSmallSpeed		= 0.01f;	//���񂾂Ƃ��̏������Ȃ鑬�x
	static constexpr float			DieMinSmallSize		= 0.0001f;	//���񂾂Ƃ��̍ŏ��̏�����



public:
	CZako2();
	virtual ~CZako2() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	void RayUpdate() override;

	void DrawShadowMap() override;

	bool GetIsAttack() { return m_IsAttack; }

	void SetIsAttack(bool isAttack) { m_IsAttack = isAttack; }


public:
	enum enZako2Motion
	{
		Z2_M_Stop,		//�~�܂�
		Z2_M_Move,		//�ړ�
		Z2_M_Attack,	//�U��
		Z2_M_RightSride,//�E�X���C�h
		Z2_M_LeftSride,	//���X���C�h
		Z2_M_Die,		//����ł���
	};

	enum enZako2Animation
	{
		Z2_A_Wait = 0,			//�ҋ@
		Z2_A_Move,				//�ړ�
		Z2_A_StreetSride,		//�O�i�X���C�h
		Z2_A_ArmAttack,			//���r�U��
		Z2_A_RightSride,			//���X���C�h
		Z2_A_LeftSride,		//�E�X���C�h
		Z2_A_Jump,				//�W�����v
		Z2_A_Wait2,				//�ҋ@�Q
		Z2_A_FallDown,			//�|���
		Z2_A_GetUp,				//�N���オ��
		Z2_A_OneArmAttack,		//�Иr�U��
	};

private:

	void ChangeMotion(int Motion = 0, int Animation = 0, double ASpeed = AnimSpeed);

	//���[�V�����I��
	void ChoiceRandomMotion();

private:

	bool		m_IsAttack;		//�U�����Ă��邩�ǂ���

	int			m_SrideTime;	//�X���C�h����


};

