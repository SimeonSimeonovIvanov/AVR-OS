@echo OFF
call AvrBuild.bat
for %%t in (*.map *.tmp *.lst bin\*.obj bin\*.tmp) do del %%t
pause