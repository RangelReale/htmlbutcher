<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

<xsl:param name="glossterm.auto.link" select="1" />
<xsl:param name="glossentry.show.acronym" select="yes" />
<xsl:param name="admon.graphics" select="1" />
<!--<xsl:param name="navig.graphics" select="1" />-->
<xsl:param name="callout.graphics.number.limit" select="'15'"></xsl:param>


<xsl:param name="admon.graphics.path" select="'images/'"></xsl:param>
<xsl:param name="callout.graphics.path" select="'images/callouts/'"></xsl:param>
<xsl:param name="html.stylesheet" select="'images/html1.css'"></xsl:param>
<xsl:param name="html.stylesheet.type">text/css</xsl:param>
<xsl:param name="html.cleanup" select="1"></xsl:param>
<xsl:param name="css.decoration" select="1"></xsl:param>
<xsl:param name="highlight.source" select="1"/>


<xsl:param name="generate.toc">
appendix  toc,title
article/appendix  nop
article   toc,title
book      toc,title,figure,table,example,equation
chapter   toc,title
part      toc,title
preface   toc,title
qandadiv  toc
qandaset  toc
reference toc,title
sect1     toc
sect2     toc
sect3     toc
sect4     toc
sect5     toc
section   toc
set       toc,title
</xsl:param>


</xsl:stylesheet>

