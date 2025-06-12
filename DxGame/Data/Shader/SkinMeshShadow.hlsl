/***************************************************************************************************
*	SkinMeshCode Version 2.40
*	LastUpdate	: 2024/06/14.
**/
//------------------------------------------------.
//	�萔.
//------------------------------------------------.
static const int MAX_BONE_MATRICES = 255;

//------------------------------------------------.
//	�O���[�o���ϐ�.
//------------------------------------------------.
Texture2D g_Texture : register(t0); //�e�N�X�`���[�́A���W�X�^�[t(n).
Texture2D g_ShadowMap : register(t1);

SamplerState g_Sampler : register(s0); //�T���v���[�́A���W�X�^�[s(n).

//------------------------------------------------.
//	�R���X�^���g�o�b�t�@.
//------------------------------------------------.
//���b�V��.
cbuffer per_mesh : register(b0)
{
	float4x4 g_mW; //���[���h�s��.
	float4x4 g_mWVP; //���[���h,�r���[,�v���W�F�N�V�����̍����s��.
	float4x4 g_mWLVP; //���[���h,���C�g�r���[,�v���W�F�N�V�����̍����s��.
};
//�}�e���A��.
cbuffer per_material : register(b1)
{
	float4 g_Ambient; //�A���r�G���g�F(���F).
	float4 g_Diffuse; //�f�B�t���[�Y�F(�g�U���ːF).
	float4 g_Specular; //�X�y�L�����F(���ʔ��ːF).
};
//�t���[��.
cbuffer per_frame : register(b2)
{
	float4 g_CameraPos; //�J�����ʒu(�n�_�ʒu).
	float4 g_vLightDir; //���C�g�̕����x�N�g��.
};
//�{�[���̃|�[�Y�s�񂪓���.
cbuffer per_bones : register(b3)
{
	float4x4 g_mConstBoneWorld[MAX_BONE_MATRICES];
};

//�X�L�j���O��̒��_�E�@��������.
struct Skin
{
	float4 Pos;
	float4 Norm;
};
//�o�[�e�b�N�X�o�b�t�@�[�̓���.
struct VSSkinIn
{
	float3 Pos : POSITION; //�ʒu.  
	float3 Norm : NORMAL; //���_�@��.
	float2 UV : TEXCOORD; //UV���W�i�e�N�X�`���[���W�j.
	uint4 Bones : BONE_INDEX; //�{�[���̃C���f�b�N�X.
	float4 Weights : BONE_WEIGHT; //�{�[���̏d��.
};

//�s�N�Z���V�F�[�_�[�̓��́i�o�[�e�b�N�X�o�b�t�@�[�̏o�́j�@
struct PSSkinIn
{
	float4 Pos : SV_Position; //�ʒu.
	float3 Norm : TEXCOORD0; //���_�@��.
	float2 UV : TEXCOORD1; //UV���W�i�e�N�X�`���[���W�j.
	float3 Light : TEXCOORD2; //���C�g.
	float3 EyeVector : TEXCOORD3; //�����x�N�g��.
	float4 PosWorld : TEXCOORD4; //���[���h���W.
	float4 Color : COLOR; //�ŏI�J���[�i���_�V�F�[�_�[�ɂ����Ắj.
	float4 LightViewPos : TEXCOORD1;
};


//�w�肵���ԍ��̃{�[���̃|�[�Y�s���Ԃ�.
//	�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j.
matrix FetchBoneMatrix(uint Bone)
{
	return g_mConstBoneWorld[Bone];
}


//���_���X�L�j���O�i�{�[���ɂ��ړ��j����.
//	�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j.
Skin SkinVert(VSSkinIn Input)
{
	Skin Output = (Skin) 0;

	float4 Pos = float4(Input.Pos, 1.f); //���W��w�� 1.
	float4 Norm = float4(Input.Norm, 0.f); //�x�N�g����w�� 0.
	//�{�[��0.
	uint Bone = Input.Bones.x;
	float Weight = Input.Weights.x;
	float4x4 m = FetchBoneMatrix(Bone);
	Output.Pos += Weight * mul(Pos, m);
	Output.Norm += Weight * mul(Norm, m);
	
	//�{�[��1.
	Bone = Input.Bones.y;
	Weight = Input.Weights.y;
	m = FetchBoneMatrix(Bone);
	Output.Pos += Weight * mul(Pos, m);
	Output.Norm += Weight * mul(Norm, m);
	
	//�{�[��2.
	Bone = Input.Bones.z;
	Weight = Input.Weights.z;
	m = FetchBoneMatrix(Bone);
	Output.Pos += Weight * mul(Pos, m);
	Output.Norm += Weight * mul(Norm, m);
	
	//�{�[��3.
	Bone = Input.Bones.w;
	Weight = Input.Weights.w;
	m = FetchBoneMatrix(Bone);
	Output.Pos += Weight * mul(Pos, m);
	Output.Norm += Weight * mul(Norm, m);

	return Output;
}

// �o�[�e�b�N�X�V�F�[�_.
float4 VS_Main(VSSkinIn input) : SV_POSITION
{
	PSSkinIn output = (PSSkinIn) 0;
	
	Skin vSkinned = SkinVert(input);

	//�v���W�F�N�V�����ϊ��i���[���h�A�r���[�A�v���W�F�N�V�����j.
	output.Pos = mul(vSkinned.Pos, g_mWVP);
	
	//�@�������f���̎p���ɍ��킹��.
	// (���f������]����Ζ@������]������K�v�����邽��).
	output.Norm = normalize((float3) mul(vSkinned.Norm, g_mW));
	
	//���C�g����:
	// �f�B���N�V���i�����C�g�́A�ǂ��ł���������.�ʒu�͖��֌W.
	output.Light = normalize(g_vLightDir.xyz);
	
	//���[���h���W.
	output.PosWorld = mul(float4(input.Pos, 1.f), g_mW);
	
	//�����x�N�g��:
	// ���[���h��ԏ�ł̒��_���璸�_�֌������x�N�g��.
	output.EyeVector = normalize(g_vLightDir.xyz - output.PosWorld.xyz); //w�ȊO�Ōv�Z.

	//UV���W�i�e�N�X�`�����W�j.	
	output.UV = input.UV;

	float4 PosWorld = mul(vSkinned.Pos, g_mW);
	
	output.LightViewPos = mul(PosWorld, g_mWLVP);
	
	return output.LightViewPos;
}

// �s�N�Z���V�F�[�_.
float4 PS_Main(float4 Pos : SV_POSITION) : SV_Target
{
	return float4(Pos.z, Pos.z, Pos.z, 1.0f);
}
