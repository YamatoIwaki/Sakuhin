#pragma once

#include <functional>
#include "CSingletonManager.h"

/*******************************************
*		�V���h�E�}�b�v�N���X
**/

//�O���錾
class CDirectX11;

class CShadowMap
	: public SingletonManager<CShadowMap>
{
private:

	CShadowMap();
	~CShadowMap();

public:

	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mWLVP;		//���[���h,�r���[,�v���W�F�N�V�����̍����ϊ��s��.	
	};


	//���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//���_���W.
		D3DXVECTOR3	Normal;	//�@��(�A�e�v�Z�ɕK�{).
	};

	//�J���[�o�b�t�@�̃N���A�J���[
	static constexpr float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };


public:
	friend class SingletonManager<CShadowMap>;

	//�����ݒ�
	HRESULT Init();

	//�V�F�[�_�쐬.
	HRESULT CreateShader();

	//���C�g����̃r���[�ˉe�s���ݒ肵�A�V���h�E�}�b�v�̐����Ɏg�p����
	void SetLightViewProj(const D3DXMATRIX& lightviewproj);

	//�V���h�E�}�b�v�̃V�F�[�_�[���\�[�X�r���[���擾
	ID3D11ShaderResourceView** GetShadowMapSRV() { return &m_ShadowMapSRV; }

	void BeginShadow();
	void EndShadow();


	ID3D11VertexShader* GetVertexShader() { return m_pVertexShader; }
	ID3D11InputLayout* GetLayoutShader() { return m_pVertexLayout;  }
	ID3D11PixelShader* GetPixcelShader() { return m_pPixelShader; }
	static ID3D11Buffer* GetConstantBuffer();
private:

	//Direct11
	CDirectX11*					m_pDx11;
	ID3D11Device*				m_pDevice11;
	ID3D11DeviceContext*		m_pContext11;

	ID3D11VertexShader*			m_pVertexShader;	//���_�V�F�[�_.
	ID3D11InputLayout*			m_pVertexLayout;	//���_���C�A�E�g.
	ID3D11PixelShader*			m_pPixelShader;		//�s�N�Z���V�F�[�_.

	//�V���h�E�}�b�v�̉𑜓x
	int							m_ShadowMapWidth;	//��
	int							m_ShadowMapHeight;	//����

	//�V���h�E�}�b�v�̃e�N�X�`�����\�[�X(�[�x���̊i�[)
	ID3D11Texture2D*			m_ShadowMapTexture;
	ID3D11Texture2D*			m_ShadowMapDepthTexture;

	//�V���h�E�}�b�v�̐[�x�X�e���V���r���[(�`��Ώۂ̐[�x���)
	ID3D11DepthStencilView*		m_ShadowMapDSV;

	//�V���h�E�}�b�v�̃V�F�[�_�[���\�[�X�r���[(���̃V�F�[�_�[����Q�Ɖ\�ɂ���)
	ID3D11ShaderResourceView*	m_ShadowMapSRV;

	ID3D11RenderTargetView*		m_ShadowRTV	= nullptr;

	//���C�g�̃r���[�ˉe�s��
	D3DXMATRIX					m_LightViewProj;

	//�R���X�^���g�o�b�t�@
	ID3D11Buffer*				m_pCBuffer;
};

