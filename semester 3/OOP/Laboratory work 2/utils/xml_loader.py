import xml.etree.ElementTree as ET


def load_xml_file(file_path):
    """
    Utility function to load and parse an XML file.
    """
    tree = ET.parse(file_path)
    return tree.getroot()
