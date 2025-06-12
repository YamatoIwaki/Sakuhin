#include "CConcentLine.h"
#include "CDirectX11.h"

//�V�F�[�_�t�@�C�����i�f�B���N�g�����܂ށj.
const TCHAR SHADER_NAME[] = _T("Data\\Shader\\ConcentLine.hlsl");

//�R���X�g���N�^.
CConcentLine::CConcentLine()
	: m_pDx11			(nullptr)
	, m_pDevice11		(nullptr)
	, m_pContext11		(nullptr)
	, m_pVertexShader	(nullptr)
	, m_pPixelShader	(nullptr)
	, m_pConstantBuffer	(nullptr)
	, m_Alpha			(1.0f)
	, m_IsDraw			(true)

	, m_EffecTime		()
	, m_EffecStartTime	()
	, m_EffecCurrentTime()
	, m_IsActive		(false)
{
}

//�f�X�g���N�^.
CConcentLine::~CConcentLine()
{
	//����������Ăяo��.
	Release();

	//�ʂ̂Ƃ���ŊǗ����Ă���̂ł����ł�nullptr�����ď�����.
	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
	m_pDx11 = nullptr;
}

//������.
//	ID3D11Device* pDevice11 �O���ō쐬���Ď����Ă���B
//	ID3D11DeviceContext* pContext11 �O���ō쐬���Ď����Ă���B
HRESULT CConcentLine::Init()
{
	m_pDx11 = CDirectX11::GetInstance();
	m_pDevice11 = m_pDx11->GetDevice();		//���Ԃ͕ʂ̂Ƃ���ɂ���.���Ƃ����L���Ă���.
	m_pContext11 = m_pDx11->GetContext();	//���Ԃ͕ʂ̂Ƃ���ɂ���.���Ƃ����L���Ă���.

	//�V�F�[�_�쐬.
	if (FAILED(CreateShader()))
	{
		return E_FAIL;
	}

	//�T���v���쐬.
	return S_OK;
}

//���.
void CConcentLine::Release()
{
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

//===========================================================
//	HLSL�t�@�C����ǂݍ��݃V�F�[�_���쐬����.
//	HLSL: High Level Shading Language �̗�.
//===========================================================
HRESULT CConcentLine::CreateShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSL����o�[�e�b�N�X�V�F�[�_�̃u���u���쐬.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//�V�F�[�_�t�@�C�����iHLSL�t�@�C���j.
			nullptr,		//�}�N����`�̔z��ւ̃|�C���^�i���g�p�j.
			nullptr,		//�C���N���[�h�t�@�C���������C���^�[�t�F�C�X�ւ̃|�C���^�i���g�p�j.
			"VS_Main",		//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
			"vs_5_0",		//�V�F�[�_�̃��f�����w�肷�镶����i�v���t�@�C���j.
			uCompileFlag,	//�V�F�[�_�R���p�C���t���O.
			0,				//�G�t�F�N�g�R���p�C���t���O�i���g�p�j.
			nullptr,		//�X���b�h �|���v �C���^�[�t�F�C�X�ւ̃|�C���^�i���g�p�j.
			&pCompiledShader,//�u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors,		//�G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
			nullptr)))		//�߂�l�ւ̃|�C���^�i���g�p�j.
	{
		_ASSERT_EXPR(false, _T("hlsl�ǂݍ��ݎ��s"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�o�[�e�b�N�X�V�F�[�_�v���쐬.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader)))	//(out)�o�[�e�b�N�X�V�F�[�_.
	{
		_ASSERT_EXPR(false, _T("�o�[�e�b�N�X�V�F�[�_�쐬���s"));
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);

	//HLSL����s�N�Z���V�F�[�_�̃u���u���쐬.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,		//�V�F�[�_�t�@�C�����iHLSL�t�@�C���j.
			nullptr,			//�}�N����`�̔z��ւ̃|�C���^�i���g�p�j.
			nullptr,			//�C���N���[�h�t�@�C���������C���^�[�t�F�C�X�ւ̃|�C���^�i���g�p�j.
			"PS_Main",			//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
			"ps_5_0",			//�V�F�[�_�̃��f�����w�肷�镶����i�v���t�@�C���j.
			uCompileFlag,		//�V�F�[�_�R���p�C���t���O.
			0,					//�G�t�F�N�g�R���p�C���t���O�i���g�p�j.
			nullptr,			//�X���b�h �|���v �C���^�[�t�F�C�X�ւ̃|�C���^�i���g�p�j.
			&pCompiledShader,	//�u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors,			//�G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
			nullptr)))			//�߂�l�ւ̃|�C���^�i���g�p�j.
	{
		_ASSERT_EXPR(false, _T("hlsl�ǂݍ��ݎ��s"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�v���쐬.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader)))	//(out)�s�N�Z���V�F�[�_.
	{
		_ASSERT_EXPR(false, _T("�s�N�Z���V�F�[�_�쐬���s"));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//�R���X�^���g�i�萔�j�o�b�t�@�쐬.
	//�V�F�[�_�ɓ���̐��l�𑗂�o�b�t�@.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//�R���X�^���g�o�b�t�@���w��.
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);	//�R���X�^���g�o�b�t�@�̃T�C�Y.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//�������݂ŃA�N�Z�X.
	cb.MiscFlags = 0;	//���̑��̃t���O�i���g�p�j.
	cb.StructureByteStride = 0;	//�\���̂̃T�C�Y�i���g�p�j.
	cb.Usage = D3D11_USAGE_DYNAMIC;	//�g�p���@�F���ڏ�������.

	//�R���X�^���g�o�b�t�@�̍쐬.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			nullptr,
			&m_pConstantBuffer)))
	{
		_ASSERT_EXPR(false, _T("�R���X�^���g�o�b�t�@�쐬���s"));
		return E_FAIL;
	}

	return S_OK;
}

//�����_�����O�p.
void CConcentLine::Render()
{
	//if (!m_IsActive) return;

	//���Ԏ擾(�A�j���[�V�����p)
	static auto start = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	float time = std::chrono::duration<float>(now - start).count();

	// ----------------------------
	// Map �ɂ��f�[�^�]��
	// ----------------------------

	//�V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	//�萔�o�b�t�@�f�[�^��ݒ�
	SHADER_CONSTANT_BUFFER cb;

	//�o�b�t�@���̃f�[�^�̏��������J�n����map.
	if (SUCCEEDED(
		m_pContext11->Map(m_pConstantBuffer,
			0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		cb.screenCenter = DirectX::XMFLOAT2(0.5f, 0.5f);	//��ʂ̒��S
		cb.time			= time;			//�o�ߎ���
		cb.lineCount	= 50.0f;		//���̖{��
		cb.alpha		= m_Alpha;		//�����x

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	//�g�p����V�F�[�_�̓o�^.
	m_pContext11->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);

	//���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�����H.
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�A���t�@�u�����h�L���ɂ���.
	m_pDx11->SetAlphaBlend(true);

	//�v���~�e�B�u�������_�����O.
	m_pContext11->Draw(4, 0);//�|���i���_4���j.

	//�A���t�@�u�����h�����ɂ���.
	m_pDx11->SetAlphaBlend(false);


}

void CConcentLine::StartEffect(float t)
{
	m_EffecStartTime	= GetTime();
	m_EffecTime			= t;
	m_IsActive			= true;
}

void CConcentLine::Draw()
{
	m_EffecCurrentTime = GetTime();
	float elapsed = m_EffecCurrentTime - m_EffecStartTime;

	if (m_IsActive)
	{
		if (elapsed < m_EffecTime)
		{
			m_Alpha = 1.0f - (elapsed / m_EffecTime);

			Render();
		}
		else
		{
			m_IsActive = false;
		}
	}
}

float CConcentLine::GetTime()
{
	//���Ԏ擾
	using namespace std::chrono;
	static auto start = steady_clock::now();
	auto now = steady_clock::now();
	return duration<float>(now - start).count();

}
