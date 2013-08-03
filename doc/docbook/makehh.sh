#!/bin/sh
#XML_CATALOG_FILES="src/htmlbutcher.cat.xml"
#XML_DEBUG_CATALOG=1

xsltproc -o htmlhelp/h.hhp style-hh.xsl src/htmlbutcher.xml
cd htmlhelp
zip -r ../htmlbutcher.htb *
cd ..

exit 0

