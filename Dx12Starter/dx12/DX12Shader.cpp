#include "DX12Shader.h"
#include "Logger.h"
#include "InputFile.h"

DX12Shader::DX12Shader(LPCSTR fileName) : bytecode({ nullptr, 0 })
{
    Logger::GetInstance().Log("Loading shader: %s\n", fileName);
    
    // Use InputFile class to read the compiled shader file
    InputFile compiledShader;
    compiledShader.Open(fileName);
    
    // Check if file was opened successfully
    if (compiledShader.GetHandle() == nullptr)
    {
        Logger::GetInstance().Log("Failed to open shader file: %s\n", fileName);
        return;
    }
    
    compiledShader.Read();
    
    // Check if read was successful
    if (compiledShader.GetSize() == 0 || compiledShader.GetBuffer() == nullptr)
    {
        Logger::GetInstance().Log("Failed to read shader file: %s\n", fileName);
        compiledShader.Close();
        return;
    }
    
    // Take ownership of the buffer from InputFile
    bytecode.BytecodeLength = compiledShader.GetSize();
    Logger::GetInstance().Log("Shader size: %d bytes\n", bytecode.BytecodeLength);
    
    bytecode.pShaderBytecode = compiledShader.GetBuffer();
    Logger::GetInstance().Log("Shader buffer: %p\n", bytecode.pShaderBytecode);
    
    compiledShader.Close();
    
    Logger::GetInstance().Log("Shader loaded successfully: %s (Size: %d bytes)\n", fileName, bytecode.BytecodeLength);
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