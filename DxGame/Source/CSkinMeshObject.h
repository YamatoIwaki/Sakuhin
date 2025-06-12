#pragma once

#include "CGameObject.h"
#include "CSkinMesh.h"
#include "CRay.h"

/************************************************************
*	スキンメッシュオブジェクトクラス.
**/
class CSkinMeshObject
	: public CGameObject
{
public:
	CSkinMeshObject();
	virtual ~CSkinMeshObject() override;

	//CGameObjectで純粋仮想関数の宣言がされてるのでこちらで定義を書く.
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	virtual void DrawShadowMap();

	virtual void QuaterDraw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera);

	//メッシュを接続する.
	void AttachMesh(CSkinMesh& pMesh);

	//メッシュを切り離す.
	void DetachMesh();


	//レイとメッシュの当たり判定.
	bool IsHitForRay(
		const RAY& pRay,			//レイ構造体.
		float* pfDistance,			//(out)距離
		D3DXVECTOR3* pvIntersect,	//(out)交差点
		D3DXVECTOR3* pNormal=nullptr);	//(out)法線(ベクトル).	
	//壁からの位置を計算する.
	void CalculatePositionFromWall(CROSSRAY* pCrossRay);

public:
	//Y軸方向へ伸ばしたレイを取得.
	RAY GetRayY() const { return *m_pRayY; }
	//十字レイを取得.
	CROSSRAY GetCrossRay() const { return *m_pCrossRay; }

	void SetCamPos(const D3DXVECTOR3& pos) { m_CamPos = pos; }

	float GetHp() { return m_Hp; }

	void Damage(float D) { m_Hp -= D; }


private:
	//交差位置のポリゴンの頂点を見つける.
	HRESULT FindVerticesOnPoly(
		LPD3DXMESH pMesh,
		DWORD dwPolyIndex,
		D3DXVECTOR3* pVertices);	//(out)頂点情報.

	//回転値調整(1周移譲しているときの調整).
	void ClampDirection(float* dir);	//再帰関数.



protected:
	CSkinMesh* m_pMesh;
	
	//アニメーション
	LPD3DXANIMATIONCONTROLLER	m_pAnimCtrl;	//アニメーションコントローラ.
	int							m_AnimNo;		//雑魚：アニメーション.
	double						m_AnimTime;		//雑魚：アニメーション経過時間.
	double						m_AnimSpeed;	//アニメーション速度.
	D3DXVECTOR3					m_BonePos;		//雑魚：ボーン座標.


	RAY*			m_pRayY;					//Y方向へ伸ばしたレイ.
	CROSSRAY*		m_pCrossRay;				//十字レイ

	D3DXVECTOR3		m_CamPos;

	D3DXMATRIX		m_mRotation;				//ワールド行列を取得

	D3DXVECTOR3		m_QuaterRot;	

	float			m_Hp;
};
