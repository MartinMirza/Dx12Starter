// Vertex Shader - Bypass
// Simply passes through the position data

float4 main(float4 pos : POSITION) : SV_POSITION
{
    return pos;
}