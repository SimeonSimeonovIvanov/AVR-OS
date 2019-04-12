@echo OFF
call AvrBuild.bat
for %%t in (*.map *.tmp *.obj *.lst) do del %%t
pause