
if "%1"=="" SET CONFIG=Release
if NOT "%1"=="" SET CONFIG=%1

"C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe" LunaDll.sln /m /nologo /p:Configuration="%CONFIG%" /p:Platfprm="Win32"
