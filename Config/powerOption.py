import os

def main():
    
    os.open('/boot/config.txt',os.O_RDWR)
    #os.open('/home/pi/Documents/SenProj/Alexa/alexaSkill.py',os.O_RDWR)

if __name__=="__main__":
    main()