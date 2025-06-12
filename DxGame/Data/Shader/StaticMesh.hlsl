
//------------------------------------------------.
//	�V���h�E�}�b�v�`��p�V�F�[�_�[.
//------------------------------------------------.

//�V���h�E�}�b�v�ƃT���v���[�̒�`
Texture2D g_Texture : register(t0);
Texture2D shadowMap : register(t1); //�V���h�E�}�b�v
SamplerState samplState : register(s0); //�V���h�E�}�b�v�p�T���v���[.

//------------------------------------------------.
//	�R���X�^���g�o�b�t�@.
//------------------------------------------------.
//���b�V��.
cbuffer per_mesh : register(b0)
{
    matrix g_mW; //���[���h�s��.
    matrix g_mWVP; //���[���h,�r���[,�v���W�F�N�V�����̍����s��.
    matrix g_mWLVP; //���[���h,�r���[,�v���W�F�N�V�����̍����s��.
    matrix g_mWLPT; //���[���h,�r���[,�v���W�F�N�V�����̍����s��.

};

//�}�e���A���P��.
cbuffer per_material : register(b1)
{
    float4 g_Diffuse; //�f�B�t���[�Y�F(�g�U���ːF).
    float4 g_Ambient; //�A���r�G���g�F(���F).
    float4 g_Specular; //�X�y�L�����F(���ʔ��ːF).
};
//�t���[���P��.
cbuffer per_frame : register(b2)
{
    float4 g_CameraPos; //�J�����ʒu(���_�ʒu).
    float4 g_vLightDir; //���C�g�̕����x�N�g��.
};

//���_�V�F�[�_�̏o�̓p�����[�^.
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
//	���_�V�F�[�_�[
//------------------------------------------------.
VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float4 Norm : NORMAL,
	float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

	//�v���W�F�N�V�����ϊ�(���[���h,�r���[,�v���W�F�N�V����).
    output.Pos = mul(Pos, g_mWVP);

	//�@�������f���̎p���ɍ��킹��.
	// (���f������]����Ζ@������]������K�v�����邽��).
    output.Normal = mul(Norm, (float3x3) g_mW);
    output.Normal = normalize(output.Normal);

	//���C�g����:
	// �f�B���N�V���i�����C�g�́A�ǂ��ł���������.�ʒu�͖��֌W.
    output.Light = normalize(g_vLightDir);

    output.PosWorld = mul(Pos, g_mW);

	//�����x�N�g��:
	// ���[���h��ԏ�ł̒��_���璸�_�֌������x�N�g��.
    output.EyeVector = normalize(g_CameraPos - output.PosWorld);

	//�e�N�X�`�����W.
    output.UV = UV;
			
	//���C�g�̃r���[�ˉe�s����g���ăV���h�E���W���v�Z
    output.LightTexCoord = mul(Pos, g_mWLPT);
	//���C�g�r���[�ɂ�����ʒu
    output.LightViewPos = mul(Pos, g_mWLVP);
	
    return output;
	
};


//------------------------------------------------.
//	�s�N�Z���V�F�[�_�[ 
//------------------------------------------------.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	//�e�N�X�`���J���[.
    float4 texColor = g_Texture.Sample(samplState, input.UV);

	//�����@���P.
    float4 ambient = texColor * 0.4f;

	//�g�U���ˌ� ���Q.
    float NL = saturate(dot(input.Normal, input.Light));
    float4 diffuse = (g_Diffuse / 2 + texColor / 2) * NL;

	//���ʔ��ˌ� ���R.
    float3 reflect = normalize(2 * NL * input.Normal - input.Light);
    float4 specular =
		pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;

	//�ŏI�F�@���P�C�Q�C�R�̍��v.
    float4 Color = ambient + diffuse + specular;
			
	//�V���h�E���W�̐��K��
    input.LightTexCoord /= input.LightTexCoord.w; //z���W�̔񓯎����Z
	
	//�V���h�E�}�b�v����T���v�����擾(�[�x�l���擾)
    float depth = shadowMap.Sample(samplState, input.LightTexCoord.xy).r;
    float length = input.LightViewPos.z / input.LightViewPos.w;
		
    if (input.LightTexCoord.x < 0.0f || input.LightTexCoord.x > 1.0f
		|| input.LightTexCoord.y < 0.05f || input.LightTexCoord.y > 1.0f)
    {
		//�������Ȃ�
    }
	//���݂̃s�N�Z���̐[�x�ƃV���h�E�}�b�v����擾�����[�x��r
    else if (length > depth + 0.000001f)		//�o�C�A�X��ǉ����ăV���h�E�̐��x�����P
    {
        Color *= 0.5f; //�e������ꍇ�̓V���h�E�̋���������
    }
	
    return Color;
};


//========= �e�N�X�`�������p ========================================
//-------------------------------------------------
//	���_(�o�[�e�b�N�X)�V�F�[�_.
//-------------------------------------------------
VS_OUTPUT VS_NoTex(
	float4 Pos : POSITION,
	float4 Norm : NORMAL)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

	//�v���W�F�N�V�����ϊ�(���[���h,�r���[,�v���W�F�N�V����).
    output.Pos = mul(Pos, g_mWVP);

	//�@�������f���̎p���ɍ��킹��.
	// (���f������]����Ζ@������]������K�v�����邽��).
    output.Normal = mul(Norm, (float3x3) g_mW);
    output.Normal = normalize(output.Normal);

	//���C�g����:
	// �f�B���N�V���i�����C�g�́A�ǂ��ł���������.�ʒu�͖��֌W.
    output.Light = normalize(g_vLightDir);

    output.PosWorld = mul(Pos, g_mW);

	//�����x�N�g��:
	// ���[���h��ԏ�ł̒��_���璸�_�֌������x�N�g��.
    output.EyeVector = normalize(g_CameraPos - output.PosWorld);
			
	//���C�g�̃r���[�ˉe�s����g���ăV���h�E���W���v�Z
    output.LightTexCoord = mul(Pos, g_mWLPT);
	//���C�g�r���[�ɂ�����ʒu
    output.LightViewPos = mul(Pos, g_mWLVP);
	
    return output;
}

//-------------------------------------------------
//	�s�N�Z���V�F�[�_.
//-------------------------------------------------
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	//�e�N�X�`���J���[.
	//�����@���P.
    float4 ambient = g_Ambient;

	//�g�U���ˌ� ���Q.
    float NL = saturate(dot(input.Normal, input.Light));
    float4 diffuse = g_Diffuse * NL;

	//���ʔ��ˌ� ���R.
    float3 reflect = normalize(2 * NL * input.Normal - input.Light);
    float4 specular =
		pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;

	//�ŏI�F�@���P�C�Q�C�R�̍��v.
    float4 Color = ambient + diffuse + specular;
		
	//�V���h�E���W�̐��K��
    input.LightTexCoord /= input.LightTexCoord.w; //z���W�̔񓯎����Z
    input.LightTexCoord = input.LightTexCoord * 0.5f + 0.5f; //0~1�͈̔͂ɕϊ�
	
	//�V���h�E�}�b�v����T���v�����擾(�[�x�l���擾)
    float depth = shadowMap.Sample(samplState, input.LightTexCoord.xy).r;
    float length = input.LightViewPos.z / input.LightViewPos.w;
	
    float2 shadowMapUV = input.LightViewPos.xy / input.LightViewPos.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
	
    if (input.LightTexCoord.x >= 0.0f && input.LightTexCoord.x <= 1.0f
		&& input.LightTexCoord.y >= 0.0f && input.LightTexCoord.y <= 1.0f)
    {
	//���݂̃s�N�Z���̐[�x�ƃV���h�E�}�b�v����擾�����[�x��r
        if (length > depth + 0.05f)		//�o�C�A�X��ǉ����ăV���h�E�̐��x�����P
        {
            Color *= 0.5f; //�e������ꍇ�̓V���h�E�̋���������
        }
    }
    return Color;
}
