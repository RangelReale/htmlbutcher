<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

<!--<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/fo/docbook.xsl"/>-->

<xsl:param name="glossterm.auto.link" select="1" />
<xsl:param name="glossentry.show.acronym" select="yes" />
<xsl:param name="shade.verbatim" select="1" />
<xsl:param name="admon.graphics" select="1" />
<xsl:param name="navig.graphics" select="1" />

<xsl:param name="body.font.family" select="sans-serif" />

<xsl:param name="admon.graphics.path" select="'src/images/'"></xsl:param>
<xsl:param name="callout.graphics.path" select="'src/images/callouts/'"></xsl:param>



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

<xsl:param name="qanda.inherit.numeration" select="0" />

<xsl:param name="passivetex.extensions" select="1" />




</xsl:stylesheet>

