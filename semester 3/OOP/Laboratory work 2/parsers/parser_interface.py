from abc import ABC, abstractmethod


class ParserStrategy(ABC):
    """The Strategy interface for XML parsers."""


    @abstractmethod
    def parse(self, file_path):
        """Parse the XML file and return the data in a structured format."""
        pass


    @abstractmethod
    def search(self, data, criteria):
        """Search the parsed data based on the given criteria."""
        pass
