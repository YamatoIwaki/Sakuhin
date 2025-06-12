#pragma once

#include "CGameObject.h"
#include "CStaticMesh.h"
#include "CRay.h"

/************************************************************
*	スタティックメッシュオブジェクトクラス.
**/
class CStaticMeshObject
	: public CGameObject
{
public:
	CStaticMeshObject();
	virtual ~CStaticMeshObject() override;

	//CGameObjectで純粋仮想関数の宣言がされてるのでこちらで定義を書く.
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw( D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera ) override;

	void DrawShadowMap();

	//メッシュを接続する.
	void AttachMesh( CStaticMesh& pMesh ){
		m_pMesh = &pMesh;
	}
	//メッシュを切り離す.
	void DetachMesh(){
		m_pMesh = nullptr;
	}

	//レイとメッシュの当たり判定.
	bool IsHitForRay(
		const RAY& pRay,			//レイ構造体.
		float* pfDistance,			//(out)距離
		D3DXVECTOR3* pvIntersect,	//(out)交差点
		D3DXVECTOR3* pNormal=nullptr);	//(out)法線(ベクトル).	
	//壁からの位置を計算する.
	void CalculatePositionFromWall(CROSSRAY* pCrossRay);

private:
	//交差位置のポリゴンの頂点を見つける.
	HRESULT FindVerticesOnPoly(
		LPD3DXMESH pMesh,
		DWORD dwPolyIndex,
		D3DXVECTOR3* pVertices);	//(out)頂点情報.

	//回転値調整(1周移譲しているときの調整).
	void ClampDirection(float* dir);	//再帰関数.
protected:
	CStaticMesh*		m_pMesh;
};
