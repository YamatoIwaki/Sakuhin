#include "CConcentLine.h"
#include "CDirectX11.h"

//シェーダファイル名（ディレクトリも含む）.
const TCHAR SHADER_NAME[] = _T("Data\\Shader\\ConcentLine.hlsl");

//コンストラクタ.
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

//デストラクタ.
CConcentLine::~CConcentLine()
{
	//解放処理を呼び出す.
	Release();

	//別のところで管理しているのでここではnullptrを入れて初期化.
	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
	m_pDx11 = nullptr;
}

//初期化.
//	ID3D11Device* pDevice11 外部で作成して持ってくる。
//	ID3D11DeviceContext* pContext11 外部で作成して持ってくる。
HRESULT CConcentLine::Init()
{
	m_pDx11 = CDirectX11::GetInstance();
	m_pDevice11 = m_pDx11->GetDevice();		//実態は別のところにある.他とも共有している.
	m_pContext11 = m_pDx11->GetContext();	//実態は別のところにある.他とも共有している.

	//シェーダ作成.
	if (FAILED(CreateShader()))
	{
		return E_FAIL;
	}

	//サンプラ作成.
	return S_OK;
}

//解放.
void CConcentLine::Release()
{
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

//===========================================================
//	HLSLファイルを読み込みシェーダを作成する.
//	HLSL: High Level Shading Language の略.
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

	//HLSLからバーテックスシェーダのブロブを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//シェーダファイル名（HLSLファイル）.
			nullptr,		//マクロ定義の配列へのポインタ（未使用）.
			nullptr,		//インクルードファイルを扱うインターフェイスへのポインタ（未使用）.
			"VS_Main",		//シェーダエントリーポイント関数の名前.
			"vs_5_0",		//シェーダのモデルを指定する文字列（プロファイル）.
			uCompileFlag,	//シェーダコンパイルフラグ.
			0,				//エフェクトコンパイルフラグ（未使用）.
			nullptr,		//スレッド ポンプ インターフェイスへのポインタ（未使用）.
			&pCompiledShader,//ブロブを格納するメモリへのポインタ.
			&pErrors,		//エラーと警告一覧を格納するメモリへのポインタ.
			nullptr)))		//戻り値へのポインタ（未使用）.
	{
		_ASSERT_EXPR(false, _T("hlsl読み込み失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「バーテックスシェーダ」を作成.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader)))	//(out)バーテックスシェーダ.
	{
		_ASSERT_EXPR(false, _T("バーテックスシェーダ作成失敗"));
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);

	//HLSLからピクセルシェーダのブロブを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,		//シェーダファイル名（HLSLファイル）.
			nullptr,			//マクロ定義の配列へのポインタ（未使用）.
			nullptr,			//インクルードファイルを扱うインターフェイスへのポインタ（未使用）.
			"PS_Main",			//シェーダエントリーポイント関数の名前.
			"ps_5_0",			//シェーダのモデルを指定する文字列（プロファイル）.
			uCompileFlag,		//シェーダコンパイルフラグ.
			0,					//エフェクトコンパイルフラグ（未使用）.
			nullptr,			//スレッド ポンプ インターフェイスへのポインタ（未使用）.
			&pCompiledShader,	//ブロブを格納するメモリへのポインタ.
			&pErrors,			//エラーと警告一覧を格納するメモリへのポインタ.
			nullptr)))			//戻り値へのポインタ（未使用）.
	{
		_ASSERT_EXPR(false, _T("hlsl読み込み失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「ピクセルシェーダ」を作成.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader)))	//(out)ピクセルシェーダ.
	{
		_ASSERT_EXPR(false, _T("ピクセルシェーダ作成失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//コンスタント（定数）バッファ作成.
	//シェーダに特定の数値を送るバッファ.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//コンスタントバッファを指定.
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);	//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//書き込みでアクセス.
	cb.MiscFlags = 0;	//その他のフラグ（未使用）.
	cb.StructureByteStride = 0;	//構造体のサイズ（未使用）.
	cb.Usage = D3D11_USAGE_DYNAMIC;	//使用方法：直接書き込み.

	//コンスタントバッファの作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			nullptr,
			&m_pConstantBuffer)))
	{
		_ASSERT_EXPR(false, _T("コンスタントバッファ作成失敗"));
		return E_FAIL;
	}

	return S_OK;
}

//レンダリング用.
void CConcentLine::Render()
{
	//if (!m_IsActive) return;

	//時間取得(アニメーション用)
	static auto start = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	float time = std::chrono::duration<float>(now - start).count();

	// ----------------------------
	// Map によるデータ転送
	// ----------------------------

	//シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	//定数バッファデータを設定
	SHADER_CONSTANT_BUFFER cb;

	//バッファ内のデータの書き換え開始時にmap.
	if (SUCCEEDED(
		m_pContext11->Map(m_pConstantBuffer,
			0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		cb.screenCenter = DirectX::XMFLOAT2(0.5f, 0.5f);	//画面の中心
		cb.time			= time;			//経過時間
		cb.lineCount	= 50.0f;		//線の本数
		cb.alpha		= m_Alpha;		//透明度

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	//使用するシェーダの登録.
	m_pContext11->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);

	//このコンスタントバッファをどのシェーダで使うか？.
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//プリミティブ・トポロジーをセット.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//アルファブレンド有効にする.
	m_pDx11->SetAlphaBlend(true);

	//プリミティブをレンダリング.
	m_pContext11->Draw(4, 0);//板ポリ（頂点4つ分）.

	//アルファブレンド無効にする.
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
	//時間取得
	using namespace std::chrono;
	static auto start = steady_clock::now();
	auto now = steady_clock::now();
	return duration<float>(now - start).count();

}
