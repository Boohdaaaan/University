import xml.etree.ElementTree as ET
from parsers.parser_interface import ParserStrategy


class LINQParser(ParserStrategy):
    """
    LINQ-like XML parser implementation using ElementTree with queries.
    """

    def parse(self, file_path):
        tree = ET.parse(file_path)
        root = tree.getroot()
        scientists = []
        for sci in root.findall('Scientist'):
            scientist = {}
            scientist["Name"] = sci.find('Name').text
            faculty = {}
            faculty["Department"] = sci.find('Faculty/Department').text
            faculty["Branch"] = sci.find('Faculty/Branch').text
            scientist["Faculty"] = faculty
            scientist["ScientificDegree"] = sci.find('ScientificDegree').text
            scientist["TeachingFromDates"] = sci.find('TeachingFromDates').text
            scientists.append(scientist)
        return scientists


    def search(self, data, criteria):
        """
        Search through the data using the given criteria.
        """
        # Using list comprehensions for LINQ-like querying
        filtered = data
        for key, value in criteria.items():
            if key == "Faculty/Department":
                filtered = [s for s in filtered if s.get("Faculty", {}).get("Department", "").lower() == value.lower()]
            elif key == "Faculty/Branch":
                filtered = [s for s in filtered if s.get("Faculty", {}).get("Branch", "").lower() == value.lower()]
            else:
                filtered = [s for s in filtered if s.get(key, "").lower() == value.lower()]
        return filtered
