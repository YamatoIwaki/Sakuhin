#pragma once

//�x���ɂ��ẴR�[�h���͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning(disable:4005)

//_declspec()	:DLL����i�֐��A�N���X�A�N���X�̃����o�֐��j�G�N�X�|�[�g����.
//align()		:�i�����I�Ɂj16byte�Ŏg�p����.
#define ALIGN16	_declspec( align(16) )

#include "CSingletonManager.h"

//�O���錾.
class CDirectX11;

/**************************************************
*	�X�v���C�g2D�N���X.
**/
class CConcentLine
	: public SingletonManager<CConcentLine>
{
public:
	//======================================
	//	�\����.
	//======================================
	//�R���X�^���g�o�b�t�@�̃A�v�����̒�`.
	//���V�F�[�_���̃R���X�^���g�o�b�t�@�ƈ�v���Ă���K�v����.
	struct SHADER_CONSTANT_BUFFER
	{
		ALIGN16	DirectX::XMFLOAT2 screenCenter;		//���S.	
		ALIGN16 float lineCount;		//���C����.
		ALIGN16 float time;				//����.
		ALIGN16 float alpha;				//����.

	};
	//���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//���_���W.
		D3DXVECTOR2	Tex;	//�e�N�X�`�����W.
	};
private:
	CConcentLine();	//�R���X�g���N�^.
public:
	friend class SingletonManager<CConcentLine>;

	~CConcentLine();	//�f�X�g���N�^.

	//������.
	HRESULT Init();

	//���.
	void Release();

	//�V�F�[�_�쐬.
	HRESULT CreateShader();

	//�����_�����O�p.
	void Render();

	void StartEffect(float t = 0.3f);

	void Draw();

	float GetTime();


	void SetAlpha(float a) { m_Alpha = a; }

	void SetDraw(bool d) { m_IsDraw = d; }

private:
	CDirectX11*				m_pDx11;
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;

	ID3D11VertexShader*		m_pVertexShader;	//���_�V�F�[�_.
	ID3D11PixelShader*		m_pPixelShader;		//�s�N�Z���V�F�[�_.
	ID3D11Buffer*			m_pConstantBuffer;	//�R���X�^���g�o�b�t�@.

	float					m_Alpha;
	bool					m_IsDraw;

	float					m_EffecTime;
	float					m_EffecStartTime;
	float					m_EffecCurrentTime;
	bool					m_IsActive;


};