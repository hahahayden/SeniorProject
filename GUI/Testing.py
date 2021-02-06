import os


my_file=open("/boot/config.txt","w")

content = my_file.read()

my_file.close()
print(content)