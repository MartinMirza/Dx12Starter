#include "DX12Shader.h"
#include "Logger.h"
#include "InputFile.h"

DX12Shader::DX12Shader(LPCSTR fileName) : bytecode({ nullptr, 0 })
{
    Logger::GetInstance().Log("Loading shader: %s\n", fileName);
    
    // Use InputFile class to read the compiled shader file
    InputFile compiledShader;
    compiledShader.Open(fileName);
    compiledShader.Read();
    compiledShader.Close();
    
    // Take ownership of the buffer from InputFile
    bytecode.BytecodeLength = compiledShader.GetSize();
    bytecode.pShaderBytecode = compiledShader.GetBuffer();
    
    Logger::GetInstance().Log("Shader loaded successfully: %s\n", fileName);
}

DX12Shader::~DX12Shader()
{
    // This class is responsible for freeing the buffer
    delete (char*)bytecode.pShaderBytecode;
}

D3D12_SHADER_BYTECODE DX12Shader::GetBytecode()
{
    return bytecode;
}