/*********************************************************************
*	集中戦クラス用シェーダファイル.
**/

//コンスタントバッファ.
//アプリ側と同じバッファサイズになっている必要がある.
cbuffer per_mesh : register(b0) //レジスタ番号.
{
    float2  screenCenter    : packoffset( c0 ); //中心
    float   lineCount       : packoffset( c1 );      //集中戦本数
    float   time            : packoffset( c2 );           //アニメーション時間
    float   falpha           : packoffset( c3 );
};

//頂点シェーダの出力パラメータ.
//頂点シェーダで制御した複数の値をピクセルシェーダ側に渡すために用意している.
struct VS_OUTPUT
{
    float4 Pos : SV_Position; //座標（SV_:System-Value Semantics）.
    float2 UV : TEXCOORD0; //UV座標.
};


Texture2D colorTexture : register(t0);

//疑似ランダムなグラディエントノイズを生成する関数
float GradientNoise(float2 uv, float scale)
{
    //スケーリングして細かくする
    uv *= scale;
    
    //セルの左上整数座標
    float2 i = floor(uv);
    
    //セル内のローカル座標(0~1)
    float2 f = frac(uv);
    
    //疑似ランダムなグラディエント方向(4つの頂点分)
    float2 r00 = normalize(float2(
        frac(sin(dot(i + float2(0.0f, 0.0f), float2(127.1f, 311.7f))) * 43758.5453),
        frac(sin(dot(i + float2(0.0f, 0.0f), float2(269.5f, 183.3f))) * 43758.5453)
    ));
    
    float2 r10 = normalize(float2(
        frac(sin(dot(i + float2(1.0f, 0.0f), float2(127.1f, 311.7f))) * 43758.5453),
        frac(sin(dot(i + float2(1.0f, 0.0f), float2(269.5f, 183.3f))) * 43758.5453)
    ));

    float2 r01 = normalize(float2(
        frac(sin(dot(i + float2(0.0f, 1.0f), float2(127.1f, 311.7f))) * 43758.5453),
        frac(sin(dot(i + float2(0.0f, 1.0f), float2(269.5f, 183.3f))) * 43758.5453)
    ));

    float2 r11 = normalize(float2(
        frac(sin(dot(i + float2(1.0f, 1.0f), float2(127.1f, 311.7f))) * 43758.5453),
        frac(sin(dot(i + float2(1.0f, 1.0f), float2(269.5f, 183.3f))) * 43758.5453)
    ));

    //各グラディエントとオフセットベクトルのどっと積(重み)
    float d00 = dot(r00, f - float2(0.0f, 0.0f));
    float d10 = dot(r10, f - float2(1.0f, 0.0f));
    float d01 = dot(r01, f - float2(0.0f, 1.0f));                                              
    float d11 = dot(r11, f - float2(1.0f, 1.0f));
    
    //補間(スムージング関数)
    float2 u = f * f * (3.0f - 2.0f * f);   //Smoothstep-like
    
    //バイリニア補間(上下、左右)
    float nx0 = lerp(d00, d10, u.x);
    float nx1 = lerp(d01, d11, u.x);
    float nxy = lerp(nx0, nx1, u.y);

    //－1～＋1に収まるノイズ値を返す
    return nxy;
    
}


//頂点シェーダ.
//主にモデルの頂点の表示位置を決定する.
VS_OUTPUT VS_Main(uint id : SV_VertexID)
{
    float2 pos[] =
    {
        float2(-1.0f, -1.0f),
        float2(-1.0f, 1.0f),
        float2(1.0f, -1.0f),
        float2(1.0f, 1.0f),
    };
    
    float2 uv = (pos[id] + 1.0f) * 0.5;
    
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = float4(pos[id], 0.0f, 1.0f);
    output.UV = uv;
    return output;
   
}

//ピクセルシェーダ.
//画面上に評されるピクセル（ドット1つ分）の色を決定する.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
    //UV空間での中心からのベクトル
    float2 delta = (input.UV - screenCenter);
    
    //角度を取得(-π～-π)
    float angle = atan2(delta.x, delta.y);
    float rad = (angle / (3.14) + 1.0f) / 2.0f;
    
    float noise = GradientNoise(
    float2(rad, rad) + float2(time / 5.0f, 0.0f), lineCount
    );
    
    float y = 1.0f - 0.2f + noise;
    
    //線を浮かび上がらせる(0.95以上を白線に)   
    float concentLine = smoothstep(0.8f, 1.0f, y);
    
    //中心からの距離によるフェード
    float dist = length(delta);
    float fade = saturate(dist * 1.0f);
    float radialFade = smoothstep(0.2, 1.0f, dist);
    
    float alpha = concentLine * radialFade;
    
    return float4(alpha, alpha, alpha, alpha);
}