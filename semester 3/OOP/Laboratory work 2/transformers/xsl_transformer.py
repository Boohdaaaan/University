from lxml import etree


class XSLTransformer:
    """
    Handles transformation of XML data to HTML using XSLT.
    """


    def __init__(self, xsl_path):
        """
        Initialize the transformer with the path to the XSLT file.
        """
        self.xsl = etree.parse(xsl_path)
        self.transform = etree.XSLT(self.xsl)


    def transform_to_html(self, data, output_path):
        """
        Transform the filtered data to HTML and save it.
        """
        # Create an XML structure from the data
        root = etree.Element("Scientists")
        for scientist in data:
            sci_elem = etree.SubElement(root, "Scientist")
            name = etree.SubElement(sci_elem, "Name")
            name.text = scientist.get("Name", "")
            faculty = etree.SubElement(sci_elem, "Faculty")
            department = etree.SubElement(faculty, "Department")
            department.text = scientist.get("Faculty", {}).get("Department", "")
            branch = etree.SubElement(faculty, "Branch")
            branch.text = scientist.get("Faculty", {}).get("Branch", "")
            degree = etree.SubElement(sci_elem, "ScientificDegree")
            degree.text = scientist.get("ScientificDegree", "")
            teaching = etree.SubElement(sci_elem, "TeachingFromDates")
            teaching.text = scientist.get("TeachingFromDates", "")
        
        # Convert the XML tree to a string
        xml_str = etree.tostring(root, pretty_print=True, xml_declaration=True, encoding='UTF-8')

        # Parse the XML string
        xml_doc = etree.fromstring(xml_str)

        # Perform the transformation
        html_doc = self.transform(xml_doc)

        # Write the HTML to the output file
        with open(output_path, 'wb') as f:
            f.write(etree.tostring(html_doc, pretty_print=True, method="html"))
