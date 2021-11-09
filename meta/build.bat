@echo off

set CompilerFlags=-Od -MTd -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -W3 -D_CRT_SECURE_NO_WARNINGS -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7
set LinkerFlags= -incremental:no -opt:ref user32.lib

echo Compiling meta program...

cl %CompilerFlags% meta.cpp /link %LinkerFlags% | more
