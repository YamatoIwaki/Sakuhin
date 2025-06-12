
//------------------------------------------------.
//	シャドウマップ描画用シェーダー.
//------------------------------------------------.

//シャドウマップとサンプラーの定義
Texture2D g_Texture : register(t0);
Texture2D shadowMap : register(t1); //シャドウマップ
SamplerState samplState : register(s0); //シャドウマップ用サンプラー.

//------------------------------------------------.
//	コンスタントバッファ.
//------------------------------------------------.
//メッシュ.
cbuffer per_mesh : register(b0)
{
    matrix g_mW; //ワールド行列.
    matrix g_mWVP; //ワールド,ビュー,プロジェクションの合成行列.
    matrix g_mWLVP; //ワールド,ビュー,プロジェクションの合成行列.
    matrix g_mWLPT; //ワールド,ビュー,プロジェクションの合成行列.

};

//マテリアル単位.
cbuffer per_material : register(b1)
{
    float4 g_Diffuse; //ディフューズ色(拡散反射色).
    float4 g_Ambient; //アンビエント色(環境色).
    float4 g_Specular; //スペキュラ色(鏡面反射色).
};
//フレーム単位.
cbuffer per_frame : register(b2)
{
    float4 g_CameraPos; //カメラ位置(視点位置).
    float4 g_vLightDir; //ライトの方向ベクトル.
};

//頂点シェーダの出力パラメータ.
struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float3 Normal : TEXCOORD0;
    float2 UV : TEXCOORD1;
    float3 Light : TEXCOORD2;
    float3 EyeVector : TEXCOORD3;
    float4 PosWorld : TEXCOORD4;
    float4 LightTexCoord : TEXCOORD5;
    float4 LightViewPos : TEXCOORD6;
    float4 Color : COLOR;
};

//------------------------------------------------.
//	頂点シェーダー
//------------------------------------------------.
VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float4 Norm : NORMAL,
	float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

	//プロジェクション変換(ワールド,ビュー,プロジェクション).
    output.Pos = mul(Pos, g_mWVP);

	//法線をモデルの姿勢に合わせる.
	// (モデルが回転すれば法線も回転させる必要があるため).
    output.Normal = mul(Norm, (float3x3) g_mW);
    output.Normal = normalize(output.Normal);

	//ライト方向:
	// ディレクショナルライトは、どこでも同じ方向.位置は無関係.
    output.Light = normalize(g_vLightDir);

    output.PosWorld = mul(Pos, g_mW);

	//視線ベクトル:
	// ワールド空間上での頂点から頂点へ向かうベクトル.
    output.EyeVector = normalize(g_CameraPos - output.PosWorld);

	//テクスチャ座標.
    output.UV = UV;
			
	//ライトのビュー射影行列を使ってシャドウ座標を計算
    output.LightTexCoord = mul(Pos, g_mWLPT);
	//ライトビューにおける位置
    output.LightViewPos = mul(Pos, g_mWLVP);
	
    return output;
	
};


//------------------------------------------------.
//	ピクセルシェーダー 
//------------------------------------------------.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	//テクスチャカラー.
    float4 texColor = g_Texture.Sample(samplState, input.UV);

	//環境光　※１.
    float4 ambient = texColor * 0.4f;

	//拡散反射光 ※２.
    float NL = saturate(dot(input.Normal, input.Light));
    float4 diffuse = (g_Diffuse / 2 + texColor / 2) * NL;

	//鏡面反射光 ※３.
    float3 reflect = normalize(2 * NL * input.Normal - input.Light);
    float4 specular =
		pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;

	//最終色　※１，２，３の合計.
    float4 Color = ambient + diffuse + specular;
			
	//シャドウ座標の正規化
    input.LightTexCoord /= input.LightTexCoord.w; //z座標の非同次除算
	
	//シャドウマップからサンプルを取得(深度値を取得)
    float depth = shadowMap.Sample(samplState, input.LightTexCoord.xy).r;
    float length = input.LightViewPos.z / input.LightViewPos.w;
		
    if (input.LightTexCoord.x < 0.0f || input.LightTexCoord.x > 1.0f
		|| input.LightTexCoord.y < 0.05f || input.LightTexCoord.y > 1.0f)
    {
		//何もしない
    }
	//現在のピクセルの深度とシャドウマップから取得した深度比較
    else if (length > depth + 0.000001f)		//バイアスを追加してシャドウの精度を改善
    {
        Color *= 0.5f; //影がある場合はシャドウの強さを減少
    }
	
    return Color;
};


//========= テクスチャ無し用 ========================================
//-------------------------------------------------
//	頂点(バーテックス)シェーダ.
//-------------------------------------------------
VS_OUTPUT VS_NoTex(
	float4 Pos : POSITION,
	float4 Norm : NORMAL)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

	//プロジェクション変換(ワールド,ビュー,プロジェクション).
    output.Pos = mul(Pos, g_mWVP);

	//法線をモデルの姿勢に合わせる.
	// (モデルが回転すれば法線も回転させる必要があるため).
    output.Normal = mul(Norm, (float3x3) g_mW);
    output.Normal = normalize(output.Normal);

	//ライト方向:
	// ディレクショナルライトは、どこでも同じ方向.位置は無関係.
    output.Light = normalize(g_vLightDir);

    output.PosWorld = mul(Pos, g_mW);

	//視線ベクトル:
	// ワールド空間上での頂点から頂点へ向かうベクトル.
    output.EyeVector = normalize(g_CameraPos - output.PosWorld);
			
	//ライトのビュー射影行列を使ってシャドウ座標を計算
    output.LightTexCoord = mul(Pos, g_mWLPT);
	//ライトビューにおける位置
    output.LightViewPos = mul(Pos, g_mWLVP);
	
    return output;
}

//-------------------------------------------------
//	ピクセルシェーダ.
//-------------------------------------------------
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	//テクスチャカラー.
	//環境光　※１.
    float4 ambient = g_Ambient;

	//拡散反射光 ※２.
    float NL = saturate(dot(input.Normal, input.Light));
    float4 diffuse = g_Diffuse * NL;

	//鏡面反射光 ※３.
    float3 reflect = normalize(2 * NL * input.Normal - input.Light);
    float4 specular =
		pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;

	//最終色　※１，２，３の合計.
    float4 Color = ambient + diffuse + specular;
		
	//シャドウ座標の正規化
    input.LightTexCoord /= input.LightTexCoord.w; //z座標の非同次除算
    input.LightTexCoord = input.LightTexCoord * 0.5f + 0.5f; //0~1の範囲に変換
	
	//シャドウマップからサンプルを取得(深度値を取得)
    float depth = shadowMap.Sample(samplState, input.LightTexCoord.xy).r;
    float length = input.LightViewPos.z / input.LightViewPos.w;
	
    float2 shadowMapUV = input.LightViewPos.xy / input.LightViewPos.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
	
    if (input.LightTexCoord.x >= 0.0f && input.LightTexCoord.x <= 1.0f
		&& input.LightTexCoord.y >= 0.0f && input.LightTexCoord.y <= 1.0f)
    {
	//現在のピクセルの深度とシャドウマップから取得した深度比較
        if (length > depth + 0.05f)		//バイアスを追加してシャドウの精度を改善
        {
            Color *= 0.5f; //影がある場合はシャドウの強さを減少
        }
    }
    return Color;
}
