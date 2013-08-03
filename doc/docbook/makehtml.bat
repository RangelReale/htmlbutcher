rem @echo off

rem set XML_CATALOG_FILES="file://c:/prog/personal/HTMLButcher/doc/docbook/src/htmlbutcher.cat"
rem set XML_CATALOG_FILES="src/htmlbutcher.cat.xml"
rem set XML_CATALOG_FILES="C:\prog\personal\HTMLButcher\doc\docbook\src\htmlbutcher.cat"
set LIBXSLT=C:\prog\src\docbook\libxslt-1.1.23+.win32\bin

set XML_DEBUG_CATALOG=1

%LIBXSLT%\xsltproc.exe -o html\h.html style-html.xsl src\htmlbutcher.xml
