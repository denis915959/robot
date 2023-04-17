import smbus
import time
import os
#command=1
bus = smbus.SMBus(1)
SLAVE_ADDRESS = 0x20       #04
class robot_path_node:
    def __init__(self, action1, counter1):
        self.action=action1
        self.counter=counter1

def writenumber(num):
    bus.write_byte(SLAVE_ADDRESS, int(num))
def write_array(a, b, c, d):
    bus.write_i2c_block_data(SLAVE_ADDRESS, a, [b, c, d])
def readnumber():
    num=int(bus.read_byte(SLAVE_ADDRESS))#|bus.read_byte(SLAVE_ADDRESS)#read_byte

    return num

robot_action=[]
robot_action.append(robot_path_node(5, 1))
robot_action.append(robot_path_node(0, 1))
robot_action.append(robot_path_node(6, 1))
robot_action.append(robot_path_node(0, 1))
robot_action.append(robot_path_node(5, 1))
robot_action.append(robot_path_node(0, 2))
robot_action.append(robot_path_node(2, 1))

sz_path=len(robot_action)
writenumber(sz_path)
for i in range(0, sz_path):
    writenumber(robot_action[i].action)
    writenumber(robot_action[i].counter)
    print(robot_action[i].action, "  ", robot_action[i].counter)

status=102
while(status==102):
    status=readnumber()
    print(status)
    if(status==101):#success
        print("success")
        break
    if(status<101):
        print("barrier")
        break
    time.sleep(0.1)
        
"""while True:
    
    com = input("nunbers 1 or 2:")
    command=int(com) #33 - start command
    writenumber(command)
    
    require=0 #0 - no require
    while(require==0):
        require=readnumber()
        print(require)
        time.sleep(0.1)
    print("require = ", require)
    if(require==1):
        print("red")
        writenumber(0)
    time.sleep(1)
    #expect KeyboardInterrupt:
        #quit()
#sudo i2cdetect -y 1
"""

    
