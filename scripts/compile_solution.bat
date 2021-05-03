@REM Load the Visual Studio developer command prompt
call  "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

@REM Compile the LaTeX2AI solutions
msbuild "..\LaTeX2AI.sln" /nologo /t:rebuild /p:Configuration=%LaTeX2AI_build_type% /p:Platform=x64 
