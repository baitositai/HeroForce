//�s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    //���W�i�v���W�F�N�V������ԁj
    float4 Position : SV_POSITION;
    //�f�B�t�[�Y�J���[
    float4 Diffuse : COLORO;
    float4 Spe : COLOR1;
    //float3 color : COLOR1;    // �J�X�^���f�[�^
    //�e�N�X�`�����W
    float2 TexCoords0 : TEXCOORD0;
};

//�`�悷��e�N�X�`��
Texture2D g_SrcTexture : register(t0);

//�T���v���[�F�K�؂ȐF�����߂�
SamplerState g_SrcSampler : register(s0);


//�萔�o�b�t�@�F�X���b�g�ԍ�3�Ԗځib3�j
cbuffer cbParam : register(b3)
{
    float g_time;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //UV���W���󂯎��
    float2 uv = PSInput.TexCoords0;

    //UV���W�ƃe�N�X�`�����Q�Ƃ��āA�œK�ȐF���擾����
    float4 srcCol =
        g_SrcTexture.Sample(g_SrcSampler, uv);

    return float4(uv.y, uv.y, uv.y, 1.0f);
    return float4(uv.x, uv.x, uv.x, 1.0f);
    return srcCol;
    return float4(1.0f, 0.0f, 0.0f, 1.0f);

    float intensity = abs(sin(g_time)); // ���Ԃɉ������_�Ō���
    float3 yellow = float3(1.0, 1.0, 0.0); // ���F
    srcCol.rgb = lerp(srcCol.rgb, yellow, intensity); // ���̐F�Ɖ��F����
    return srcCol;
}