# Прості рядки з подвійними лапками
path1 = "C:\\Users\\Documents\\file.txt"
path2 = "D:\\Projects\\project\\main.py"
path3 = "\\\\server\\share\\folder\\file.txt"

# Прості рядки з одинарними лапками
path4 = 'C:\\Windows\\System32\\cmd.exe'
path5 = '.\\relative\\path\\to\\file.txt'
path6 = '..\\parent\\folder\\script.py'

# Потрійні лапки
multiline_path = """
C:\Users\Documents\very\long\path\to\file.txt
D:\Another\Path\Here
"""

# Raw-рядки
raw_path1 = r"C:\Users\Documents\file.txt"
raw_path2 = r"D:\Projects\test\data.txt"
raw_path3 = r"\\server\share\folder\file.txt"
raw_path4 = R"C:\Windows\System32\config.ini"

raw_path5 = r'C:\Temp\output.log'
raw_path6 = r'.\local\file.txt'

raw_multiline = r"""
C:\Users\Documents\very\long\path\to\file.txt
D:\Another\Path\Here
"""

# F-рядки
user_name = "John"
f_path1 = f"C:\\Users\\{user_name}\\Documents\\file.txt"
f_path2 = f"D:\\Projects\\{user_name}\\main.py"
f_path3 = F"\\\\server\\share\\{user_name}\\file.txt"

# F-рядки з потрійними лапками
f_multiline = f"""
C:\\Users\\{user_name}\\Documents\\very\\long\\path\\to\\file.txt
D:\\Another\\Path\\Here
"""

# Коментарі
# C:\This\Should\Not\Be\Detected

# C:\Users\Test\file.txt

print("Шляхи успішно визначені!")
