@echo OFF
call AvrBuild.bat
for %%t in (*.map *.tmp *.lst *.obj *.tmp) do del %%t
pause