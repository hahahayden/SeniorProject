import xlrd
 
# Give the location of the file
loc = ("/home/pi/Documents/SenProj/ParseExcel/product.xls")
 
# To open Workbook
wb = xlrd.open_workbook(loc)
sheet = wb.sheet_by_index(0)
 
# For row 0 and column 0
print(sheet.cell_value(0, 0))

