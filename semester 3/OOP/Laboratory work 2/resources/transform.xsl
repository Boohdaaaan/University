<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:template match="/Scientists">
        <html>
            <head>
                <title>Scientist Personnel</title>
                <style>
                    table {width: 100%; border-collapse: collapse;}
                    th, td {border: 1px solid black; padding: 8px; text-align: left;}
                    th {background-color: #f2f2f2;}
                </style>
            </head>
            <body>
                <h2>Scientist Personnel</h2>
                <table>
                    <tr>
                        <th>Name</th>
                        <th>Department</th>
                        <th>Branch</th>
                        <th>Scientific Degree</th>
                        <th>Teaching From Dates</th>
                    </tr>
                    <xsl:for-each select="Scientist">
                        <tr>
                            <td><xsl:value-of select="Name"/></td>
                            <td><xsl:value-of select="Faculty/Department"/></td>
                            <td><xsl:value-of select="Faculty/Branch"/></td>
                            <td><xsl:value-of select="ScientificDegree"/></td>
                            <td><xsl:value-of select="TeachingFromDates"/></td>
                        </tr>
                    </xsl:for-each>
                </table>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>
