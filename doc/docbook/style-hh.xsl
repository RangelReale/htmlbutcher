<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/htmlhelp/htmlhelp.xsl"/>

<xsl:import href="style.xsl"/>

<xsl:param name="html.stylesheet" select="'images/html1.css'"></xsl:param>
<xsl:param name="admon.graphics.path" select="'images/'"></xsl:param>
<xsl:param name="callout.graphics.path" select="'images/callouts/'"></xsl:param>
<xsl:param name="htmlhelp.chm" select="'htmlbutcher.chm'"/>
<xsl:param name="ignore.image.scaling" select="1" />
<xsl:param name="suppress.navigation" select="0" />
<xsl:param name="use.id.as.filename" select="1" />

</xsl:stylesheet>

