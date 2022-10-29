KEYS = "ivC4WfVHnMU!0g3a"

from http.client import responses
from time import sleep
from tkinter import Tk
from tkinter.filedialog import askopenfilename
import  serial

Tk().withdraw() # we don't want a full GUI, so keep the root window from appearing

print("please select hex file:")
filePath = askopenfilename() # show an "Open" dialog box and return the path to the selected file

print("please enter serial port: ")
serialPort = input()

print("please enter serial baudrate: ")
serialBaudrate = int(input())

MCU = serial.Serial(port=serialPort, baudrate=serialBaudrate, timeout=1)

file = open(filePath, "r")

# wait for MCU to start connection
response = 0
while(response != bytes('S', 'utf-8')[0]):
    while(MCU.inWaiting() == 0):
        pass
    response = MCU.read()[0]

# unlock programming mode:
MCU.write(bytes(KEYS, 'utf-8'))
sleep(0.2)

def send_line(line):
    byteArr = bytes(line, 'utf-8')
    if (len(byteArr) == 0):
        return
    if (byteArr[0] != bytes(':', 'utf-8')[0]):
        return
    print("sending line:", byteArr)
    MCU.write(byteArr)
    # wait for response
    while(MCU.inWaiting() == 0):
        pass
    response = MCU.read()[0]
    # if rsponse is 'F', send line again:
    if (response == bytes('F', 'utf-8')[0]):
        send_line(line)
    # else if it is 'N', return
    elif (response == bytes('N', 'utf-8')[0]):
        return
    #else, it's error
    else:
        print("error")
        while(1):
            pass

line = file.readline()
while(line):
    # send line
    send_line(line)
    # read line from file
    line = file.readline()

print("program sent ok")