@echo off
rem call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
echo ------------------
echo +  x64 - Debug
echo ------------------
msbuild gtl.sln /p:Platform=x64 /p:Configuration=Debug -m
if %ERRORLEVEL% neq 0  goto :error_end
echo ------------------
echo +  x64 - Release
echo ------------------
msbuild gtl.sln /p:Platform=x64 /p:Configuration=Release -m
if %ERRORLEVEL% neq 0  goto :error_end
echo ------------------
echo +  win32 - Debug
echo ------------------
msbuild gtl.sln /p:Platform=win32 /p:Configuration=Debug -m
if %ERRORLEVEL% neq 0  goto :error_end
echo ------------------
echo +  win32 - Release
echo ------------------
msbuild glt.sln /p:Platform=win32 /p:Configuration=Release -m
if %ERRORLEVEL% neq 0  goto :error_end

goto :end

:error_end
@echo ----------- ERROR -----------

:end
pause
