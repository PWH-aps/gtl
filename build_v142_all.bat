@echo off
md bin
md lib
rem call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
echo ------------------
echo +  x64 - Debug
echo ------------------
msbuild gtl.sln /p:Platform=x64 /p:Configuration=Debug.v142 -m
if %ERRORLEVEL% neq 0  goto :error_end
echo ------------------
echo +  x64 - Release
echo ------------------
msbuild gtl.sln /p:Platform=x64 /p:Configuration=Release.v142 -m
if %ERRORLEVEL% neq 0  goto :error_end
rem echo ------------------
rem echo +  win32 - Debug
rem echo ------------------
rem msbuild gtl.sln /p:Platform=x86 /p:Configuration=Debug.v142 -m
rem if %ERRORLEVEL% neq 0  goto :error_end
rem echo ------------------
rem echo +  win32 - Release
rem echo ------------------
rem msbuild gtl.sln /p:Platform=x86 /p:Configuration=Release.v142 -m
rem if %ERRORLEVEL% neq 0  goto :error_end

goto :end

:error_end
@echo ----------- ERROR -----------

:end
pause
