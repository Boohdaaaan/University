import xml.sax
from parsers.parser_interface import ParserStrategy


class SAXHandler(xml.sax.ContentHandler):
    """
    Custom SAX handler to parse scientist data.
    """
    def __init__(self):
        self.current_element = ""
        self.current_scientist = {}
        self.faculty = {}
        self.scientists = []


    def startElement(self, tag, attributes):
        self.current_element = tag
        if tag == "Scientist":
            self.current_scientist = {}
            self.faculty = {}


    def endElement(self, tag):
        if tag == "Faculty":
            self.current_scientist["Faculty"] = self.faculty
        if tag == "Scientist":
            self.scientists.append(self.current_scientist)
        self.current_element = ""


    def characters(self, content):
        if self.current_element == "Name":
            self.current_scientist["Name"] = content.strip()
        elif self.current_element == "Department":
            self.faculty["Department"] = content.strip()
        elif self.current_element == "Branch":
            self.faculty["Branch"] = content.strip()
        elif self.current_element == "ScientificDegree":
            self.current_scientist["ScientificDegree"] = content.strip()
        elif self.current_element == "TeachingFromDates":
            self.current_scientist["TeachingFromDates"] = content.strip()


class SAXParser(ParserStrategy):
    """
    SAX-based XML parser implementation.
    """


    def parse(self, file_path):
        handler = SAXHandler()
        xml.sax.parse(file_path, handler)
        return handler.scientists


    def search(self, data, criteria):
        """
        Search through the data using the given criteria.
        """
        filtered = []
        for scientist in data:
            match = True
            for key, value in criteria.items():
                if key == "Faculty/Department":
                    if scientist.get("Faculty", {}).get("Department", "").lower() != value.lower():
                        match = False
                        break
                elif key == "Faculty/Branch":
                    if scientist.get("Faculty", {}).get("Branch", "").lower() != value.lower():
                        match = False
                        break
                else:
                    if scientist.get(key, "").lower() != value.lower():
                        match = False
                        break
            if match:
                filtered.append(scientist)
        return filtered
