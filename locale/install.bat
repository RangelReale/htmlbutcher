@echo off

for %%f in (*.mo) do (
mkdir %%f.dir
copy %%f %%f.dir\htmlbutcher.mo
copy ..\deps\wx\locale\%%f %%f.dir\wxstd.mo
)