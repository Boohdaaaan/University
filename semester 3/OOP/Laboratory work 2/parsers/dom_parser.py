import xml.dom.minidom
from parsers.parser_interface import ParserStrategy


class DOMParser(ParserStrategy):
    """DOM-based XML parser implementation."""

    def parse(self, file_path):
        dom = xml.dom.minidom.parse(file_path)
        scientists = []
        for sci in dom.getElementsByTagName("Scientist"):
            scientist = {}
            scientist["Name"] = sci.getElementsByTagName("Name")[0].firstChild.nodeValue
            faculty = {}
            faculty["Department"] = sci.getElementsByTagName("Department")[0].firstChild.nodeValue
            faculty["Branch"] = sci.getElementsByTagName("Branch")[0].firstChild.nodeValue
            scientist["Faculty"] = faculty
            scientist["ScientificDegree"] = sci.getElementsByTagName("ScientificDegree")[0].firstChild.nodeValue
            scientist["TeachingFromDates"] = sci.getElementsByTagName("TeachingFromDates")[0].firstChild.nodeValue
            scientists.append(scientist)
        return scientists


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
