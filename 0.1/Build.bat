@echo OFF
call AvrBuild.bat
for %%t in (*.map *.tmp *.obj) do del %%t
pause