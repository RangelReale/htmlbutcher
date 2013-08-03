@echo off

set LIBXSLT=C:\prog\src\docbook\libxslt-1.1.23+.win32\bin

%LIBXSLT%\xsltproc.exe -o htmlhelp\h.hhp style-hh.xsl src\htmlbutcher.xml
