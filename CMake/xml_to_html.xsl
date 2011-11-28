<?xml version="1.0" encoding="utf8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html"/>
<xsl:template match="/xml">
  <html>
    <head>
      <title>Catergory Index</title>
      <meta name="filename" contents="CategoryIndex.html" />
    </head>

    <body>
      <xsl:for-each select="categoryindex">
        <h2><xsl:value-of select="label" /></h2>
        <hr />
        <table class="index_table">
          <tr><th>Name</th><th>Description</th></tr>
          <xsl:for-each select="item">
            <tr>
              <td>
                <xsl:element name="a">
                  <xsl:attribute name="href"><xsl:value-of select="group" />.<xsl:value-of select="name"/>.html</xsl:attribute>
                  <xsl:variable name="group_name"><xsl:value-of select="group"/></xsl:variable>
                  <xsl:variable name="proxy_name"><xsl:value-of select="name"/></xsl:variable>
                  <xsl:value-of select="/xml/proxy[group=$group_name and name=$proxy_name]/label" />
                  <span />
                </xsl:element>
              </td>
              <td>
                <xsl:variable name="group_name"><xsl:value-of select="group"/></xsl:variable>
                <xsl:variable name="proxy_name"><xsl:value-of select="name"/></xsl:variable>
                <xsl:value-of select="/xml/proxy[group=$group_name and name=$proxy_name]/documentation/brief" />
              </td>
            </tr>
          </xsl:for-each>
        </table>
      </xsl:for-each>
    </body>
  </html>

  <xsl:apply-templates select="proxy"/>
</xsl:template>

<xsl:template match="/xml/proxy">
  <html>
    <head>
      <title><xsl:value-of select="label" /></title>
      <xsl:element name="meta">
        <xsl:attribute name="name">proxy_name</xsl:attribute>
        <xsl:attribute name="contents"><xsl:value-of select="group"/>.<xsl:value-of select="name" /></xsl:attribute>
      </xsl:element>
      <xsl:element name="meta">
        <xsl:attribute name="name">filename</xsl:attribute>
        <xsl:attribute name="contents"><xsl:value-of select="group"/>.<xsl:value-of select="name" />.html</xsl:attribute></xsl:element>
    </head>
    <body>
      <h2><xsl:value-of select="label"/> (<xsl:value-of select="name"/>)</h2>
      <i><p><xsl:value-of select="documentation/brief" /></p></i>
      <div class="description"><xsl:value-of select="documentation/long" /></div>
      <table width="97%" border="2px">
        <tr bgcolor="#9acd32">
          <th>Property</th>
          <th width="60%">Description</th>
          <th width="5%">Default(s)</th>
          <th width="20%">Restrictions</th>
        </tr>

        <xsl:for-each select="property">
          <tr>
          <th><xsl:value-of select="label" /></th>
          <td><xsl:value-of select="documentation/long" /></td>
          <td><xsl:value-of select="defaults" /><span/></td>
          <td>
            <xsl:for-each select="domains/domain">
              <p>
                <xsl:value-of select="text"/>
                <xsl:for-each select="list" >
                  <ul>
                    <xsl:for-each select="item">
                      <li><xsl:value-of select="."/></li>
                    </xsl:for-each>
                  </ul>
                </xsl:for-each>
              </p>
            </xsl:for-each>
          </td>
          </tr>
        </xsl:for-each>
      </table>
    </body>
  </html>
</xsl:template>

</xsl:stylesheet>
