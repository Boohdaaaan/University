import os

import tkinter as tk
from tkinter import filedialog, messagebox, ttk
from parsers.sax_parser import SAXParser
from parsers.dom_parser import DOMParser
from parsers.linq_parser import LINQParser
from transformers.xsl_transformer import XSLTransformer
from pathlib import Path


class MainWindow:
    """
    The main GUI window for the Scientist Personnel Analyzer application.
    """

    def __init__(self, root):
        self.root = root
        self.root.title("Scientist Personnel Analyzer")
        self.root.geometry("1200x800")
        self.parser_strategy = None
        self.xml_data = None
        self.filtered_data = None

        self.create_widgets()


    def create_widgets(self):
        """
        Create and layout all widgets in the main window.
        """
        # Frame for file operations
        file_frame = ttk.LabelFrame(self.root, text="File Operations")
        file_frame.pack(fill="x", padx=10, pady=5)

        load_button = ttk.Button(file_frame, text="Load XML File", command=self.load_xml)
        load_button.pack(side="left", padx=5, pady=5)

        export_button = ttk.Button(file_frame, text="Export Results as HTML", command=self.export_html)
        export_button.pack(side="left", padx=5, pady=5)

        # Frame for parser selection
        parser_frame = ttk.LabelFrame(self.root, text="XML Parser")
        parser_frame.pack(fill="x", padx=10, pady=5)

        self.parser_var = tk.StringVar()
        parser_options = ["SAX", "DOM", "LINQ"]
        self.parser_combobox = ttk.Combobox(parser_frame, textvariable=self.parser_var, values=parser_options, state="readonly")
        self.parser_combobox.set("Select Parser")
        self.parser_combobox.pack(side="left", padx=5, pady=5)

        # Frame for search parameters
        search_frame = ttk.LabelFrame(self.root, text="Search Parameters")
        search_frame.pack(fill="x", padx=10, pady=5)

        # Attributes
        attributes = ["Name", "Faculty/Department", "Faculty/Branch", "ScientificDegree", "TeachingFromDates"]
        self.attribute_vars = {}
        for attr in attributes:
            var = tk.BooleanVar()
            chk = ttk.Checkbutton(search_frame, text=attr, variable=var, command=self.update_search_fields)
            chk.pack(side="left", padx=5, pady=5)
            self.attribute_vars[attr] = var

        # Frame for input fields
        self.input_frame = ttk.Frame(search_frame)
        self.input_frame.pack(fill="x", padx=10, pady=5)

        # Dictionary to hold entry widgets
        self.entries = {}

        # Frame for action buttons
        action_frame = ttk.Frame(self.root)
        action_frame.pack(fill="x", padx=10, pady=5)

        search_button = ttk.Button(action_frame, text="Search", command=self.perform_search)
        search_button.pack(side="left", padx=5, pady=5)

        clear_button = ttk.Button(action_frame, text="Clear", command=self.clear_search)
        clear_button.pack(side="left", padx=5, pady=5)

        # Frame for displaying results
        result_frame = ttk.LabelFrame(self.root, text="Search Results")
        result_frame.pack(fill="both", expand=True, padx=10, pady=5)

        self.result_text = tk.Text(result_frame, wrap="word")
        self.result_text.pack(fill="both", expand=True, padx=5, pady=5)

        # Bind the close event
        self.root.protocol("WM_DELETE_WINDOW", self.on_close)


    def load_xml(self):
        """
        Load an XML file and parse it using the selected parser.
        """
        file_path = filedialog.askopenfilename(title="Select XML File", filetypes=[("XML Files", "*.xml")])
        if not file_path:
            return

        parser_choice = self.parser_var.get()
        if parser_choice == "SAX":
            self.parser_strategy = SAXParser()
        elif parser_choice == "DOM":
            self.parser_strategy = DOMParser()
        elif parser_choice == "LINQ":
            self.parser_strategy = LINQParser()
        else:
            messagebox.showerror("Parser Selection Error", "Please select a valid XML parser.")
            return

        try:
            self.xml_data = self.parser_strategy.parse(file_path)
            self.filtered_data = self.xml_data
            self.display_results(self.filtered_data)
            messagebox.showinfo("Success", f"XML file '{os.path.basename(file_path)}' loaded successfully using {parser_choice} parser.")
        except Exception as e:
            messagebox.showerror("Parsing Error", f"An error occurred while parsing the XML file:\n{e}")


    def update_search_fields(self):
        """
        Update the search input fields based on selected attributes.
        """
        # Clear existing input fields
        for widget in self.input_frame.winfo_children():
            widget.destroy()
        self.entries.clear()

        # Create new input fields for selected attributes
        row = 0
        for attr, var in self.attribute_vars.items():
            if var.get():
                label = ttk.Label(self.input_frame, text=attr + ":")
                label.grid(row=row, column=0, padx=5, pady=2, sticky="e")
                entry = ttk.Entry(self.input_frame)
                entry.grid(row=row, column=1, padx=5, pady=2, sticky="w")
                self.entries[attr] = entry
                row += 1


    def perform_search(self):
        """
        Perform search on the XML data based on user input.
        """
        if not self.xml_data:
            messagebox.showwarning("No Data", "Please load an XML file first.")
            return

        # Gather search criteria
        criteria = {}
        for attr, entry in self.entries.items():
            value = entry.get().strip()
            if value:
                criteria[attr] = value

        if not criteria:
            messagebox.showwarning("No Criteria", "Please enter at least one search criterion.")
            return

        try:
            # Filter data based on criteria
            self.filtered_data = self.parser_strategy.search(self.xml_data, criteria)
            self.display_results(self.filtered_data)
            messagebox.showinfo("Search Completed", f"Found {len(self.filtered_data)} result(s) matching the criteria.")
        except Exception as e:
            messagebox.showerror("Search Error", f"An error occurred during the search:\n{e}")


    def display_results(self, data):
        """
        Display the search results in the text widget.
        """
        self.result_text.delete(1.0, tk.END)
        for scientist in data:
            self.result_text.insert(tk.END, f"Name: {scientist.get('Name')}\n")
            faculty = scientist.get('Faculty', {})
            self.result_text.insert(tk.END, f"  Department: {faculty.get('Department')}\n")
            self.result_text.insert(tk.END, f"  Branch: {faculty.get('Branch')}\n")
            self.result_text.insert(tk.END, f"Scientific Degree: {scientist.get('ScientificDegree')}\n")
            self.result_text.insert(tk.END, f"Teaching From Dates: {scientist.get('TeachingFromDates')}\n")
            self.result_text.insert(tk.END, "-"*40 + "\n")


    def clear_search(self):
        """
        Clear all search parameters and results.
        """
        for var in self.attribute_vars.values():
            var.set(False)
        self.update_search_fields()
        self.result_text.delete(1.0, tk.END)
        self.filtered_data = self.xml_data


    def export_html(self):
        """
        Export the filtered XML data as an HTML file using XSLT.
        """
        if not self.filtered_data:
            messagebox.showwarning("No Data", "There is no data to export.")
            return

        # Ask user where to save the HTML file
        file_path = filedialog.asksaveasfilename(defaultextension=".html",
                                                 filetypes=[("HTML Files", "*.html")],
                                                 title="Save as HTML")
        if not file_path:
            return

        try:
            xsl_path = Path(__file__).parent.parent / "resources" / "transform.xsl"
            transformer = XSLTransformer(xsl_path) 
            transformer.transform_to_html(self.filtered_data, file_path)
            messagebox.showinfo("Export Successful", f"Results exported successfully to '{os.path.basename(file_path)}'.")
        except Exception as e:
            messagebox.showerror("Export Error", f"An error occurred during export:\n{e}")


    def on_close(self):
        """
        Handle the window close event with a confirmation dialog.
        """
        if messagebox.askyesno("Exit Confirmation", "Are you sure you want to exit the program?"):
            self.root.destroy()
