import logging
import os
 
from flask import Flask
from flask_ask import Ask, request, session, question, statement
import RPi.GPIO as GPIO
 
from flask import render_template

app = Flask(__name__,template_folder='templates')

ask = Ask(app, "/"
logging.getLogger('flask_ask').setLevel(logging.DEBUG)
 
STATUSON = ["on", "switch on", "enable", "power on", "activate", "turn on"] # all values that are defined as synonyms in type
STATUSOFF = ["off", "switch off", "disactivate", "turn off", "disable", "turn off"]
 
PILLHISTORY=["history","recent ones","recent"] 
PILLSCHEDULE=["time","schedule"]
@ask.launch
def launch():)
    speech_text = 'Welcome to the Raspberry Pi alexa automation.'
    return question(speech_text).reprompt(speech_text).simple_card(speech_text)
 
    
@ask.intent('historyPill', mapping = {'history':'history'})
def history_Intent(history,room):
 
    
    if history in PILLHISTORY:
     
        count=0
        #CHANGE THE DRUGFILE.TXT file
        
        f=open("drugFile.txt","r")
        Lines=f.readlines()
        #for line in Lines:
        #   count+=1 
        #  print(line)
            
        fullStatement='This is your pill history   ' 
  
        notice=render_template('templates2.yaml',title=fullStatement,lin=Lines)
        return statement(notice)
    else:
        return statement('Sorry, this command is not possible. Please resay your command.')


@ask.intent('schedPill', mapping = {'schedule':'schedule'})   

def history_Intent(schedule,room):
    #notice=render_template('templates.yaml')
    #return statement(notice)
 
    if schedule in PILLSCHEDULE:

        count=0
        
        #Pill Schedule to take pills
        f=open("schedule.txt","r")
        Lines=f.readlines()
        #for line in Lines:
        #    count+=1 
        #    print(line)
            
        fullStatement='This is your schedule   ' 
        #for line in Lines:
        #   count+=1 
        #   fullStatement=fullStatement+ line

            #return statement(fullStatement)
        notice=render_template('templates.yaml',title=fullStatement,lin=Lines)
        
        return statement(notice)

    else:
        return statement('Sorry, this command is not possible. Please resay your command.')
 
 
 
 
 
@ask.intent('AMAZON.HelpIntent')
def help():
    speech_text = 'You can say hello to me!'
    return question(speech_text).reprompt(speech_text).simple_card('HelloWorld', speech_text)
 
 
@ask.session_ended
def session_ended():
    return "{}", 200
 
 
if __name__ == '__main__':
    if 'ASK_VERIFY_REQUESTS' in os.environ:
        verify = str(os.environ.get('ASK_VERIFY_REQUESTS', '')).lower()
        if verify == 'false':
            app.config['ASK_VERIFY_REQUESTS'] = False
    app.run(debug=True)