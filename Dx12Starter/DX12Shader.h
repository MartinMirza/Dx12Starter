#pragma once

#include <d3d12.h>
#include <string>

class DX12Shader
{
public:
    DX12Shader(LPCSTR fileName);
    ~DX12Shader();
    D3D12_SHADER_BYTECODE GetBytecode();

private:
    D3D12_SHADER_BYTECODE bytecode;
};