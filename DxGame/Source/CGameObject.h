#pragma once
#include "CSingletonManager.h"
#include "CCapsuleCollision.h"
#include "CSoundManager.h"
#include "CConcentLine.h"
#include "CCamera.h"

/**************************************************
*	�Q�[���I�u�W�F�N�g�N���X.
**/
class CGameObject
{
public:
	CGameObject();
	//�p�����s���ꍇ�́A�f�X�g���N�^��virtual��t���ĉ��z�֐��ɂ��邱��.
	virtual ~CGameObject();

	//virtual �^ �֐��� = 0; �������z�֐�.
	//�q�N���X�ɏ��������C������̂ŁA�����ł͖��O�����錾���Ē�`�͏����Ȃ�.
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw( D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera ) = 0;

	//���W�ݒ�֐�.
	void SetPosition( float x, float y, float z ){
		m_vPosition.x = x;
		m_vPosition.y = y;
		m_vPosition.z = z;
	};
	void SetPosition( const D3DXVECTOR3& pos ){
		m_vPosition = pos;
	}
	//���W�擾�֐�.
	const D3DXVECTOR3& GetPosition() const {
		return m_vPosition;
	}

	//��]�ݒ�֐�.
	void SetRotation( float x, float y, float z ){
		m_vRotation.x = x;
		m_vRotation.y = y;
		m_vRotation.z = z;
	};
	void SetRotation( const D3DXVECTOR3& rot ){
		m_vRotation = rot;
	}
	//��]�擾�֐�.
	const D3DXVECTOR3& GetRotation() const {
		return m_vRotation;
	}

	//�g�k�ݒ�֐�.
	void SetScale( float x, float y, float z ){
		m_vScale.x = x;
		m_vScale.y = y;
		m_vScale.z = z;
	}
	void SetScale( float xyz ){
		m_vScale = D3DXVECTOR3( xyz, xyz, xyz );
	}
	//�g�k�擾�֐�.
	const D3DXVECTOR3& GetScale() const {
		return m_vScale;
	}

	//�����蔻��
	CCapsuleCollision::Capsule& GetCapsule() { return m_Capsule; }

	//�L�[���͂����Z�b�g(�J�ڏI�����ɓ����Ƃ悢)
	void ResetKeyStates()
	{
		for (int vk = 0x01; vk <= 0xFE; ++vk)
		{
			GetAsyncKeyState(vk);	//����������
		}
	}


	void SetScene(enScene scene) { m_Scene = scene; }


protected://protected�͎q�N���X�̂݃A�N�Z�X�\.
	D3DXVECTOR3	m_vPosition;
	D3DXVECTOR3	m_vRotation;
	D3DXVECTOR3	m_vScale;
	float		m_Alpha;

	CCapsuleCollision::Capsule m_Capsule;

	enScene		m_Scene;

};