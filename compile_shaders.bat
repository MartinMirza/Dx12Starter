@echo off
setlocal

REM Set up the environment for HLSL compilation
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

REM Compile all vertex shaders (*_vs.hlsl)
for %%f in (Dx12Starter\*_vs.hlsl) do (
    echo Compiling vertex shader: %%f
    fxc /T vs_5_0 /E main /Fo "%%~nf.cso" "%%f"
    if %ERRORLEVEL% neq 0 (
        echo Vertex shader compilation failed: %%f
        exit /b %ERRORLEVEL%
    )
    copy "%%~nf.cso" Dx12Starter\"%%~nf.cso"
)

REM Compile all pixel shaders (*_ps.hlsl)
for %%f in (Dx12Starter\*_ps.hlsl) do (
    echo Compiling pixel shader: %%f
    fxc /T ps_5_0 /E main /Fo "%%~nf.cso" "%%f"
    if %ERRORLEVEL% neq 0 (
        echo Pixel shader compilation failed: %%f
        exit /b %ERRORLEVEL%
    )
    copy "%%~nf.cso" Dx12Starter\"%%~nf.cso"
)

echo Shader compilation completed successfully
endlocal