line_number = 0
list_of_results = []
# Open the file in read only mode
imprint=open("imprintResults.txt","r")
imprintCon=imprint.read()
imprintCon=imprintCon.split('\n')

for x in range(0,len(imprintCon)):
    with open('imprints2.txt', 'r') as read_obj:
        # Read all lines in the file one by one
    

            for line in read_obj:
                # For each line, check if line contains the string
                
                line_number += 1
                for i in range(len(line)):
                    if(line[i] == ';'):
                        result_line = line[0:i] + line[i + 1:len(line)]
                        break
            
                
                #if ';' in line:
                #    line=line.replace(';','')
                #    print(line)
                if imprintCon[x] in result_line:
                    # If yes, then add the line number & line as a tuple in the list
                    list_of_results.append((line_number, line.rstrip()))
                
print(list_of_results)


f=open('resultingParameters.txt',"w")

f.write('PillHistory\n\n')
for y in range(0,len(list_of_results)):
    #print(list_of_results[y][1])
    list_of_results2=list_of_results[y][1].split('  ')
    f.write("Pill Found In Database Line:  "+str(list_of_results[y][0])+ "  ")
    
    f.write("PillDescription:  "+ str(list_of_results2[0])+ "  ")
    f.write("Pill Imprint:  "+str(list_of_results2[1])+ "  ")
    f.write('\n')