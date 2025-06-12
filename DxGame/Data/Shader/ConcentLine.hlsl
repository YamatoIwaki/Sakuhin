/*********************************************************************
*	�W����N���X�p�V�F�[�_�t�@�C��.
**/

//�R���X�^���g�o�b�t�@.
//�A�v�����Ɠ����o�b�t�@�T�C�Y�ɂȂ��Ă���K�v������.
cbuffer per_mesh : register(b0) //���W�X�^�ԍ�.
{
    float2  screenCenter    : packoffset( c0 ); //���S
    float   lineCount       : packoffset( c1 );      //�W����{��
    float   time            : packoffset( c2 );           //�A�j���[�V��������
    float   falpha           : packoffset( c3 );
};

//���_�V�F�[�_�̏o�̓p�����[�^.
//���_�V�F�[�_�Ő��䂵�������̒l���s�N�Z���V�F�[�_���ɓn�����߂ɗp�ӂ��Ă���.
struct VS_OUTPUT
{
    float4 Pos : SV_Position; //���W�iSV_:System-Value Semantics�j.
    float2 UV : TEXCOORD0; //UV���W.
};


Texture2D colorTexture : register(t0);

//�^�������_���ȃO���f�B�G���g�m�C�Y�𐶐�����֐�
float GradientNoise(float2 uv, float scale)
{
    //�X�P�[�����O���čׂ�������
    uv *= scale;
    
    //�Z���̍��㐮�����W
    float2 i = floor(uv);
    
    //�Z�����̃��[�J�����W(0~1)
    float2 f = frac(uv);
    
    //�^�������_���ȃO���f�B�G���g����(4�̒��_��)
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

    //�e�O���f�B�G���g�ƃI�t�Z�b�g�x�N�g���̂ǂ��Ɛ�(�d��)
    float d00 = dot(r00, f - float2(0.0f, 0.0f));
    float d10 = dot(r10, f - float2(1.0f, 0.0f));
    float d01 = dot(r01, f - float2(0.0f, 1.0f));                                              
    float d11 = dot(r11, f - float2(1.0f, 1.0f));
    
    //���(�X���[�W���O�֐�)
    float2 u = f * f * (3.0f - 2.0f * f);   //Smoothstep-like
    
    //�o�C���j�A���(�㉺�A���E)
    float nx0 = lerp(d00, d10, u.x);
    float nx1 = lerp(d01, d11, u.x);
    float nxy = lerp(nx0, nx1, u.y);

    //�|1�`�{1�Ɏ��܂�m�C�Y�l��Ԃ�
    return nxy;
    
}


//���_�V�F�[�_.
//��Ƀ��f���̒��_�̕\���ʒu�����肷��.
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

//�s�N�Z���V�F�[�_.
//��ʏ�ɕ]�����s�N�Z���i�h�b�g1���j�̐F�����肷��.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
    //UV��Ԃł̒��S����̃x�N�g��
    float2 delta = (input.UV - screenCenter);
    
    //�p�x���擾(-�΁`-��)
    float angle = atan2(delta.x, delta.y);
    float rad = (angle / (3.14) + 1.0f) / 2.0f;
    
    float noise = GradientNoise(
    float2(rad, rad) + float2(time / 5.0f, 0.0f), lineCount
    );
    
    float y = 1.0f - 0.2f + noise;
    
    //���𕂂��яオ�点��(0.95�ȏ�𔒐���)   
    float concentLine = smoothstep(0.8f, 1.0f, y);
    
    //���S����̋����ɂ��t�F�[�h
    float dist = length(delta);
    float fade = saturate(dist * 1.0f);
    float radialFade = smoothstep(0.2, 1.0f, dist);
    
    float alpha = concentLine * radialFade;
    
    return float4(alpha, alpha, alpha, alpha);
}