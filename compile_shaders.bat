@echo off
setlocal

REM Set up the environment for HLSL compilation
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

REM Compile vertex shader
fxc /T vs_5_0 /E main /Fo bypass_vs.cso Dx12Starter\bypass_vs.hlsl
if %ERRORLEVEL% neq 0 (
    echo Vertex shader compilation failed
    exit /b %ERRORLEVEL%
)

REM Compile pixel shader
fxc /T ps_5_0 /E main /Fo bypass_ps.cso Dx12Starter\bypass_ps.hlsl
if %ERRORLEVEL% neq 0 (
    echo Pixel shader compilation failed
    exit /b %ERRORLEVEL%
)

echo Shader compilation completed successfully
endlocal