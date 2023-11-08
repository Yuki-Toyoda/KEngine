// ���_�f�[�^
struct Vertex
{
    float4 position;
    float2 texcoord;
    float3 normal;
    float4 color;
};

// �}�e���A��
struct Material
{
    float4x4 uvTransform;
    int enableLighting;
};

// ���s����
struct DirectionalLight
{
    float4x4 viewProjection;
    float4 color; // !< ���C�g�̐F
    float3 direction; // !< ���C�g�̌���
    float intensity; // !< �P�x
};

// �C���f�b�N�X���̍\����
struct IndexInfo
{
    uint vertex; // �������_�C���f�b�N�X�̖���
    uint cameraVP; // �J�����̃r���[�v���W�F�N�V�����s��
    uint worldMatrix; // ���[���h�g�����X�t�H�[��
    uint material; // �}�e���A��
    uint tex2d; // �e�N�X�`��
};


// ** �����p ** //

// �e�X�g���N�`���[�o�b�t�@�[�̃C���f�b�N�X
StructuredBuffer<IndexInfo> gIndex : register(t0);
// ���s����
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b0);


// ** VS�p ** //

// ���_�f�[�^
StructuredBuffer<Vertex> gVertex : register(t1);
// �J������ViewProjection
StructuredBuffer<float4x4> gCameraVP : register(t2);
// WorldTransform
StructuredBuffer<float4x4> gWorldMatrix : register(t3);


// ** PS�p ** //

// �}�e���A��
StructuredBuffer<Material> gMaterial : register(t1);

// �V���h�E�}�b�v�̃e�N�X�`��
Texture2D<float> gShadowMap : register(t2);
SamplerState gShadowMapSampler : register(s0);
// �e�N�X�`��
Texture2D<float4> gTexture[] : register(t3);
SamplerState gSampler : register(s1);


// ���_�V�F�[�_�[�̏o�͌��ʂ̍\����
struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    uint id : SV_InstanceID; // VS�ŎQ�Ƃ���IndexInfo�̔ԍ�
};