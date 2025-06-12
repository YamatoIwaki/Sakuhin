#pragma once
#include "CSkinMeshObject.h"

/*******************************************
*		�v���C���[�N���X
**/

class CKaito
	: public CSkinMeshObject
	, public SingletonManager<CKaito>
{
public:
	const D3DXVECTOR3		StartPos			= D3DXVECTOR3(0.0f, 0.0f, -10.0f);	//�Q�[�����C�������ʒu
	static constexpr double AnimSpeed			= 0.02;		//�A�j���[�V�����X�s�[�h

	static constexpr float	MaxHp				= 100.0f;	//�ő�̗�
	static constexpr float	MoveSlantCont		= 0.71f;	//�΂߈ړ�����l
	static constexpr float	RollingSpeed		= 0.2f;		//��]�ړ��X�s�[�h
	static constexpr int	RollingResetTime	= 20;
	static constexpr float	SwordDamage			= 30.0f;	//���U���̃_���[�W


public:
	//�O��ړ�
	enum enMoveAround
	{
		Move_Around_Stop = 0,	//��~

		Move_Forward,			//�O�i
		Move_Backward,			//���
	};
	//���E�ړ�
	enum enMoveAttendDance
	{
		Move_Attend_Stop = 0,	//��~

		Move_Leftward,			//���ړ�
		Move_Rightward,			//���
	};

	//�v���C���[�̃��[�V����
	enum enPlayerMotion
	{
		P_M_Stop,		//�~�܂�
		P_M_Move,		//�ړ�
		P_M_Rolling,	//��]�ړ�
		P_M_Attack1,	//�U����i�K
		P_M_Attack2,	//�U����i�K
		P_M_Attack3,	//�U���O�i�K
		P_M_Die,		//�|���
	};

	//�A�j���[�V�����i���o�[
	enum enPlayerAnim
	{
		P_A_Wait,		//�ҋ@
		P_A_Run,		//����
		P_A_Attack1,	//�U���P
		P_A_Attack2,	//�U���Q
		P_A_Attack3,	//�U���R
		P_A_Rolling,	//��]
		P_A_Magic,		//���@
		P_A_KnockBack,	//�m�b�N�o�b�N
		P_A_Dead,		//��
	};


	enum enPlayerAttackColl
	{
		P_C_Attack1 = 0,
		P_C_Attack2,
		P_C_Attack3,
	};


private:
	CKaito();
	~CKaito() override;

public:
	friend class SingletonManager<CKaito>;

	virtual void Init() override;

	virtual void Update() override;

	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	CCapsuleCollision::Capsule& GetAttackColl(int no) { return m_AttackCapsule[no]; }

	int	GetMotion() { return m_PlayerMotion; }

	bool GetIsDie() { return m_IsDie; }

private:
	//���W�R������
	void MoveControl();

	void ChangeMotion(int Motion = 0, int Animation = 0, double ASpeed = AnimSpeed);

	void AttackRange();

private:
	
	bool				m_IsDebug;

	//���[�V����
	int					m_PlayerMotion;

	//�ړ�
	enMoveAround		m_MoveAround;			//�O��ړ����.
	enMoveAttendDance	m_MoveAttendDance;		//���E�ړ����.
	float				m_MoveSpeed;		//�ړ����x.
	float				m_MoveContSpeed;		//�΂߈ړ�������
	bool				m_IsAround;				
	bool				m_IsAttend;
	float				m_DefaAngle;

	//��]�ړ�
	D3DXVECTOR3			m_RollingvecZ;
	int					m_RollingCoolTime;

	//�U�������蔻��
	std::vector<CCapsuleCollision::Capsule>		m_AttackCapsule;

	//�|�ꂽ����
	bool				m_IsDie;

};

