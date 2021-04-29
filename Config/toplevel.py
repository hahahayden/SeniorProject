import logging
import threading
import time
import os


def parseFileWordByWord(fileName):
    with open(fileName,'r') as file:
   
        # reading each line    
        for line in file:
    
            # reading each word        
            for word in line.split():
    
                # displaying the words           
                print(word) 

def thread_function(name):
    #ID=1 is doing the GUI/ LUCAS code
    if (name==1):
    
        logging.info("Thread %s: starting", name)
        os.system('homescreen.py') #run the GUI
        
        logging.info("Thread %s: finishing", name)
        
    #ID=2 is checking the file and then sending stuff to FLASH and GOOGLE CLOUD
    elif (name==2):
        logging.info("Thread %s: starting", name)
        #os.system('mtime')
        filetime=os.popen('stat -c %Y testing.py').read()
        #parseFileWordByWord('testing.txt')
        while(1):
            filetimeCheck=os.popen('stat -c %Y testing.py').read() # im not sure if this might break aarons reading on lucas file so we will see
            if(filetime!=filetimeCheck):
                filetime=filetimeCheck
                print("FileTime has been changed to", filetimeCheck)
                #I need to call on a C executable that hardcodes the databeing sent
                
                os.system('gsutil cp testing.txt gs://pilltank-test-haydentam') # upload the text file to the cloud overwriting it
                
            else:
                print("FileTime of the last edit was", filetime)
            
        #print(filetime)
        logging.info("Thread %s: finishing",name)
    
    #Everything below here is Alexa
    # need to run Alexa Skill. 
    #Advise to not run process 3 unless every 70 hours cuz once change you need to go to alexa web to fix it
    
    elif (name==3):
        logging.info("Thread %s: starting",name)
        os.system('python3 /home/pi/Documents/SenProj/Alexa/alexaSkill.py')  #CHANGE TO UR DIRECTORY AARON
        logging.info("Thread %s: finishing",name)
    #running NGROK
    elif (name==4):
        logging.info("Thread %s starting", name)
        os.system('/home/pi/ngrok http 5000')
        logging.info("Thread %s: finishing",name)

if __name__ == "__main__":
    format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=format, level=logging.INFO,
                        datefmt="%H:%M:%S")

    logging.info("Main    : before creating thread")
    x = threading.Thread(target=thread_function, args=(1,))
    y=threading.Thread(target=thread_function,args=(2,))
    #z=threading.Thread(target=thread_function,args=(3,))
    a=threading.Thread(target=thread_function,args=(4,))
    
    logging.info("Main    : before running thread")
    x.start()
    #y.start()
    #z.start()
    a.start()
    logging.info("Main    : wait for the thread to finish")
    # x.join()
    logging.info("Main    : all done")
 