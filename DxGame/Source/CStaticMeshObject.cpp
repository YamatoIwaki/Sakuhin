#include "CStaticMeshObject.h"

CStaticMeshObject::CStaticMeshObject()
	: m_pMesh		( nullptr )
{
}

CStaticMeshObject::~CStaticMeshObject()
{
	DetachMesh();
}

void CStaticMeshObject::Init()
{
}

void CStaticMeshObject::Update()
{
	if( m_pMesh == nullptr ){
		return;
	}
}

void CStaticMeshObject::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj,
	LIGHT& Light, CAMERA& Camera )
{
	if( m_pMesh == nullptr ){
		return;
	}

	//描画直前で座標や回転情報などを更新.
	m_pMesh->SetPosition( m_vPosition );
	m_pMesh->SetRotation( m_vRotation );
	m_pMesh->SetScale( m_vScale );

	//レンダリング.
	m_pMesh->Render( View, Proj, Light, Camera.Position );
}


void CStaticMeshObject::DrawShadowMap()
{
	if (m_pMesh == nullptr) {
		return;
	}

	//描画直前で座標や回転情報などを更新.
	m_pMesh->SetPosition(m_vPosition);
	m_pMesh->SetRotation(m_vRotation);
	m_pMesh->SetScale(m_vScale);

	//レンダリング.
	m_pMesh->ShadowRender();
}

bool CStaticMeshObject::IsHitForRay(const RAY& pRay, float* pfDistance, D3DXVECTOR3* pvIntersect, D3DXVECTOR3* pNormal)
{
	D3DXVECTOR3 vAxis;	//軸ベクトル,
	D3DXMATRIX	mRotationY;	//ｙ軸回転行列.

	//ｙ軸回転行列.
	D3DXMatrixRotationY(&mRotationY, pRay.RotationY);
	//ｙ軸回転行列を使って軸ベクトルの座標変換する.
	D3DXVec3TransformCoord(&vAxis, &pRay.Axis, &mRotationY);

	//レイの視点と終点.
	D3DXVECTOR3 StartPoint, EndPoint;
	StartPoint = pRay.Position;
	EndPoint = pRay.Position + (vAxis * pRay.Length);	//レイの終点を設定.

	//レイを当てたいメッシュが移動している場合でも.
	//対象のworld行列の逆行列を用いれば正しくレイが当たる.
	D3DXMATRIX mWorld, mInverseWorld;
	//移動処理.
	D3DXMATRIX mTran;
	D3DXMatrixTranslation(
		&mTran,
		m_vPosition.x, m_vPosition.y, m_vPosition.z
	);

	//回転処理.
	//※この行列計算は[CStaticMesh::Render()関数]と同じする必要あり.
	D3DXMATRIX mRot, mYaw, mPitch, mRoll;
	//Y軸回転行列作成.
	D3DXMatrixRotationY(&mYaw, m_vRotation.y);
	//X軸回転行列作成.
	D3DXMatrixRotationX(&mPitch, m_vRotation.x);
	//Z軸回転行列.
	D3DXMatrixRotationZ(&mRoll, m_vRotation.z);
	//回転行列作成.
	mRot = mYaw * mPitch * mRoll;

	//拡縮処理.
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, m_vScale.x, m_vScale.y, m_vScale.z);

	//ワールド行列計算.
	//拡縮×回転×移動 ※順番がとても大切！.
	mWorld = mScale * mRot * mTran;

	//逆行列を求める.
	D3DXMatrixInverse(&mInverseWorld, nullptr, &mWorld);
	//レイの始点、終点に反映.
	D3DXVec3TransformCoord(&StartPoint, &StartPoint, &mInverseWorld);
	D3DXVec3TransformCoord(&EndPoint, &EndPoint, &mInverseWorld);

	//向きと長さ(大きさ)を求める.
	D3DXVECTOR3 vDirection = EndPoint - StartPoint;

	BOOL bHit = FALSE;		//命中フラグ	.
	DWORD dwIndex = 0;		//インデックス番号
	D3DXVECTOR3 Vertex[3];	//頂点座標
	FLOAT U = 0, V = 0;		//重心ヒット座標.

	//メッシュとレイの交差を調べる.
	D3DXIntersect(
		m_pMesh->GetMeshForRay(),	//対象メッシュ
		&StartPoint,				//レイの始点
		&vDirection,				//レイの向きと長さ（大きさ）
		&bHit,						//(out)判定結果.
		&dwIndex,					//(out)bHitがTRUE時にレイの始点に最も近くの面のインデックス値へのポインタ
		&U, &V,						//(out)重心ヒット座標.
		pfDistance,					//(out)メッシュとの距離.
		nullptr, nullptr);			//

	//無限に伸びるレイのどこかでメッシュが当たっていたら.
	if (bHit == TRUE)
	{
		//命中した時.
		FindVerticesOnPoly(
			m_pMesh->GetMeshForRay(),
			dwIndex, Vertex);

		//重心座標から交点を算出.
		//ローカル交点は　v0 + U*(v1-v0) + V+(v2-v0)で求める
		*pvIntersect =
			Vertex[0] + U * (Vertex[1] - Vertex[0]) + V * (Vertex[2] - Vertex[0]);

		//モデルデータが「拡縮」「回転」「移動」していれば行列が必要.
		D3DXVec3TransformCoord(pvIntersect, pvIntersect, &mWorld);

		//法線の出力要求があれば
		if (pNormal != nullptr) {
			D3DXPLANE p;
			//その頂点(３点)から「平面の方程式を得る」.
			D3DXPlaneFromPoints(&p, &Vertex[0], &Vertex[1], &Vertex[2]);
			
			//平面の方程式の係数が法線の成分.
			*pNormal = D3DXVECTOR3(p.a, p.b, p.c);

			//法線は移動量計算に使用するので回転のみ処理をする
			D3DXVec3TransformCoord(pNormal, pNormal, &mRot);
		}

		//EndPointから見た距離で1.0fより小さければ当たっている.
		if (*pfDistance < 1.0f)
		{
			return true;	//命中している.
		}
	}

	return false;	//外れている.


	return false;
}

//壁からの位置を計算する.
void CStaticMeshObject::CalculatePositionFromWall(CROSSRAY* pCrossRay)
{
	static constexpr float WSPACE = 0.5f;	//壁との限界距離.

	FLOAT Distance;			//レイの距離
	D3DXVECTOR3 Intersect;	//レイの交差点.

	D3DXVECTOR3 Normal;		//法線(ベクトル).
	D3DXVECTOR3 Offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	//レイの向きによりあたら壁までの距離を求める.軸ベクトル(前後左右).
	for (int dir = 0; dir < CROSSRAY::max; dir++)
	{
		if (IsHitForRay(pCrossRay->Ray[dir], &Distance, &Intersect, &Normal))
		{
			if (Distance <= WSPACE)
			{
				//法線は壁からまっすぐに出ているので、法線とかけ合わせて,
				//対象を動かすベクトルが得られる.
				Offset = Normal * (WSPACE - Distance);

				//レイの位置を更新.
				for (int i = 0; i < CROSSRAY::max; i++)
				{
					pCrossRay->Ray[i].Position += Offset;
				}
			}
		}
	}

}

//交差位置のポリゴンの頂点を見つける.
//※頂点座標はローカル座標.
HRESULT CStaticMeshObject::FindVerticesOnPoly(
	LPD3DXMESH pMesh,
	DWORD dwPolyIndex, 
	D3DXVECTOR3* pVertices)
{
	//頂点ごとのバイト数を取得.
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	//頂点数を取得
	DWORD dwVertexAmt = pMesh->GetNumVertices();
	//面数を取得.
	DWORD dwPoly = pMesh->GetNumFaces();

	WORD* pwPoly = nullptr;

	//インデックスバッファをロック(読み込みモード).
	pMesh->LockIndexBuffer(
		D3DLOCK_READONLY,
		reinterpret_cast<VOID**>(&pwPoly));

	BYTE* pbVertices = nullptr;	//頂点(バイト型).
	FLOAT* pfVertices = nullptr;//頂点(float型).
	LPDIRECT3DVERTEXBUFFER9	VB = nullptr;	//頂点バッファ.

	//頂点情報の取得.
	pMesh->GetVertexBuffer( &VB );

	//頂点バッファのロック.
	if (SUCCEEDED(VB->Lock(0, 0, reinterpret_cast<VOID**> (&pbVertices), 0)))
	{
		//ポリゴンの頂点一つ目を取得.
		pfVertices = reinterpret_cast<FLOAT*>
			(&pbVertices[dwStride * pwPoly[dwPolyIndex * 3]]);

		pVertices[0].x = pfVertices[0];
		pVertices[0].y = pfVertices[1];
		pVertices[0].z = pfVertices[2];

		//ポリゴンの頂点二つ目を取得.
		pfVertices = reinterpret_cast<FLOAT*>
			(&pbVertices[dwStride * pwPoly[dwPolyIndex * 3 + 1]]);

		pVertices[1].x = pfVertices[0];
		pVertices[1].y = pfVertices[1];
		pVertices[1].z = pfVertices[2];

		//ポリゴンの頂点三つ目を取得.
		pfVertices = reinterpret_cast<FLOAT*>
			(&pbVertices[dwStride * pwPoly[dwPolyIndex * 3 + 2]]);

		pVertices[2].x = pfVertices[0];
		pVertices[2].y = pfVertices[1];
		pVertices[2].z = pfVertices[2];

		pMesh->UnlockIndexBuffer();	//ロック解除.
		VB->Unlock();				//ロック解除.
	}
	VB->Release();	//不要になったんで解放.


	return S_OK;
}

//回転値調整(1周以上しているときの調整).
void CStaticMeshObject::ClampDirection(float* dir)
{
	if (*dir > D3DX_PI * 2.0f)
	{
		//一周以上している/
		*dir -= D3DX_PI * 2.0f; //2π(360°)分引く.
	}

	if (*dir > D3DX_PI * 2.0f)
	{
		//再帰関数.
		ClampDirection(dir);
	}
}
