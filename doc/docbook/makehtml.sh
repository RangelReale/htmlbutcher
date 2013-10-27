#!/bin/sh
#XML_CATALOG_FILES="src/htmlbutcher.cat.xml"
#XML_DEBUG_CATALOG=1

xsltproc -o html/h.html style-html.xsl src/htmlbutcher.xml
