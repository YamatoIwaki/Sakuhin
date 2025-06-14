#include "CMain.h"
#include "CDirectX9.h"
#include "CDirectX11.h"
#include "CGame.h"

#include "CUIObject.h"
#include "CSprite2DManager.h"

//ウィンドウを画面中央で起動を有効にする.
//#define ENABLE_WINDOWS_CENTERING

//=================================================
//	定数.
//=================================================
const TCHAR WND_TITLE[] = _T( "Clean Up The Machine" );
const TCHAR APP_NAME[]	= _T( "Clean Up The Machine" );


/********************************************************************************
*	メインクラス.
**/
//=================================================
//	コンストラクタ.
//=================================================
CMain::CMain()
	//初期化リスト.
	: m_hWnd	( nullptr )
	, m_pDx9	( nullptr )
	, m_pDx11	( nullptr )
	, m_pGame	( nullptr )
{
	m_pDx9 = CDirectX9::GetInstance();
	m_pDx11 = CDirectX11::GetInstance();
}


//=================================================
//	デストラクタ.
//=================================================
CMain::~CMain()
{
	SAFE_DELETE( m_pGame );

	DeleteObject( m_hWnd );
}


//更新処理.
void CMain::Update()
{
	
	//更新処理.
	m_pGame->Update();

	//バックバッファをクリアにする.
	m_pDx11->ClearBackBuffer();

	//描画処理.
	m_pGame->Draw();
	
	//画面に表示.
	m_pDx11->Present();
}


//構築処理.
HRESULT CMain::Create()
{
	//DirectX9構築.
	if( FAILED( m_pDx9->Create( m_hWnd ) ) )
	{
		return E_FAIL;
	}
	//DirectX11構築.
	if( FAILED( m_pDx11->Create( m_hWnd ) ) )
	{
		return E_FAIL;
	}

	//ゲームクラスのインスタンス生成.
	m_pGame = new CGame( *m_pDx9, *m_pDx11, m_hWnd );

	//ゲームクラスの構築（Loadも含める）.
	m_pGame->Create();

	return S_OK;
}

//データロード処理.
HRESULT CMain::LoadData()
{
	m_pDx11->ClearBackBuffer();
	std::unique_ptr<CSprite2D> load = std::make_unique<CSprite2D>();

	CSprite2D::SPRITE_STATE ss = { WND_W, WND_H, WND_W, WND_H , WND_W, WND_H };

	load->Init(_T("Data\\Texture\\Loading.png"), ss);

	//ロードする前にロード中描画だけ先にしておく
	load->Render();

	m_pDx11->Present();

	//データロード処理.
	m_pGame->LoadData();

	return S_OK;
}


//解放処理.
void CMain::Release()
{
	if( m_pDx11 != nullptr ){
		m_pDx11->Release();
	}
	if( m_pDx9 != nullptr ){
		m_pDx9->Release();
	}
}


//メッセージループ.
void CMain::Loop()
{
	//データロード.
	if( FAILED( LoadData() )){
		return;
	}

	//------------------------------------------------
	//	フレームレート調整準備.
	//------------------------------------------------
	float Rate = 0.0f;	//レート.
	DWORD sync_old = timeGetTime();			//過去時間.
	DWORD sync_now;							//現在時間.

	//時間処理のため、最小単位を1ミリ秒に変更.
	timeBeginPeriod( 1 );
	Rate = 1000.0f / static_cast<float>(FPS); //理想時間を算出.

	//メッセージループ.
	MSG msg = { 0 };
	ZeroMemory( &msg, sizeof( msg ) );

	while( msg.message != WM_QUIT )
	{
		sync_now = timeGetTime();	//現在の時間を取得.

		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else if( sync_now - sync_old >= Rate )
		{
			sync_old = sync_now;	//現在時間に置き換え.

			//更新処理.
			Update();
		}
	}
	//アプリケーションの終了.
	Release();
}

//ウィンドウ初期化関数.
HRESULT CMain::InitWindow(
	HINSTANCE hInstance,	//インスタンス.
	INT x, INT y,			//ウィンドウx,y座標.
	INT width, INT height)	//ウィンドウ幅,高さ.
{
	//ウィンドウの定義.
	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( wc ) );//初期化(0を設定).

	wc.cbSize			= sizeof( wc );
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= MsgProc;//WndProc;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon( nullptr, IDI_APPLICATION );
	wc.hCursor			= LoadCursor( nullptr, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)GetStockObject( LTGRAY_BRUSH );
	wc.lpszClassName	= APP_NAME;
	wc.hIconSm			= LoadIcon( nullptr, IDI_APPLICATION );

	//ウィンドウクラスをWindowsに登録.
	if( !RegisterClassEx( &wc ) ) {
		_ASSERT_EXPR( false, _T( "ウィンドウクラスの登録に失敗" ) );
		return E_FAIL;
	}

	//--------------------------------------.
	//	ウィンドウ表示位置の調整.
	//--------------------------------------.
	//この関数内でのみ使用する構造体をここで定義.
	struct RECT_WND
	{
		INT x, y, w, h;
		RECT_WND() : x(), y(), w(), h() {}
	} rectWindow;//ここに変数宣言もする.

#ifdef ENABLE_WINDOWS_CENTERING
	//ディスプレイの幅、高さを取得.
	HWND hDeskWnd = nullptr;
	RECT recDisplay;
	hDeskWnd = GetDesktopWindow();
	GetWindowRect( hDeskWnd, &recDisplay );

	//センタリング.
	rectWindow.x = ( recDisplay.right - width ) / 2;	//表示位置x座標.
	rectWindow.y = ( recDisplay.bottom - height ) / 2;	//表示位置y座標.
#endif//ENABLE_WINDOWS_CENTERING

	//--------------------------------------.
	//	ウィンドウ領域の調整.
	//--------------------------------------.
	RECT	rect;		//矩形構造体.
	DWORD	dwStyle;	//ウィンドウスタイル.
	rect.top = 0;			//上.
	rect.left = 0;			//左.
	rect.right = width;		//右.
	rect.bottom = height;	//下.
	dwStyle = WS_OVERLAPPEDWINDOW;	//ウィンドウ種別.

	if( AdjustWindowRect(
		&rect,			//(in)画面サイズが入った矩形構造体.(out)計算結果.
		dwStyle,		//ウィンドウスタイル.
		FALSE ) == 0 )	//メニューを持つかどうかの指定.
	{
		MessageBox(
			nullptr,
			_T( "ウィンドウ領域の調整に失敗" ),
			_T( "エラーメッセージ" ),
			MB_OK );
		return 0;
	}

	//ウィンドウの幅高さ調節.
	rectWindow.w = rect.right - rect.left;
	rectWindow.h = rect.bottom - rect.top;


	//ウィンドウの作成.
	m_hWnd = CreateWindow(
		APP_NAME,					//アプリ名.
		WND_TITLE,					//ウィンドウタイトル.
		dwStyle,					//ウィンドウ種別(普通).
		rectWindow.x, rectWindow.y,	//表示位置x,y座標.
		rectWindow.w, rectWindow.h,	//ウィンドウ幅,高さ.
		nullptr,					//親ウィンドウハンドル.
		nullptr,					//メニュー設定.
		hInstance,					//インスタンス番号.
		nullptr );					//ウィンドウ作成時に発生するイベントに渡すデータ.
	if( !m_hWnd ) {
		_ASSERT_EXPR( false, _T( "ウィンドウ作成失敗" ) );
		return E_FAIL;
	}

	//ウィンドウの表示.
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	return S_OK;
}


//ウィンドウ関数（メッセージ毎の処理）.
LRESULT CALLBACK CMain::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam )
{
	switch( uMsg ) {
	case WM_DESTROY://ウィンドウが破棄されたとき.
		//アプリケーションの終了をWindowsに通知する.
		PostQuitMessage( 0 );
		break;

	case WM_KEYDOWN://キーボードが押されたとき.
		//キー別の処理.
		switch( static_cast<char>( wParam ) ) {
		case VK_ESCAPE:	//ESCｷｰ.
			if( MessageBox( nullptr,
				_T( "ゲームを終了しますか？" ),
				_T( "警告" ), MB_YESNO ) == IDYES )
			{
				//ウィンドウを破棄する.
				DestroyWindow( hWnd );
			}
			break;
		}
		break;
	}

	//メインに返す情報.
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}