@echo off

set LIBXSLT=C:\prog\src\docbook\libxslt-1.1.23+.win32\bin
set FOP=C:\prog\src\docbook\fop-0.95\fop.bat

%LIBXSLT%\xsltproc.exe -o pdf\h.fo style-pdf.xsl src\htmlbutcher.xml
%FOP% -fo pdf\h.fo -pdf pdf\h.pdf
