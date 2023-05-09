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
    num=int(bus.read_byte(SLAVE_ADDRESS))   #|bus.read_byte(SLAVE_ADDRESS)#read_byte
    return num
    
"""def readnumber():
    data=bus.read_i2c_block_data(SLAVE_ADDRESS, 0x00, 2) #|bus.read_byte(SLAVE_ADDRESS)#read_byte
    return data"""

mode=1

if(mode==1):
    robot_action=[]
    robot_action.append(robot_path_node(5, 1))#1
    robot_action.append(robot_path_node(2, 1))
    robot_action.append(robot_path_node(12, 0))
    robot_action.append(robot_path_node(5, 0))
    robot_action.append(robot_path_node(6, 1))
    robot_action.append(robot_path_node(0, 1))
    path=[0, 2, 3, 2, 0]

    writenumber(mode)

    sz_path=len(robot_action)
    writenumber(sz_path) #send robot_action to robot
    for i in range(0, sz_path):
        writenumber(robot_action[i].action)
        writenumber(robot_action[i].counter)
        print(robot_action[i].action, "  ", robot_action[i].counter)

    status=102
    while(status==102):
        #recv_arr=readnumber()
        #status=recv_arr[0]
        #j_recv=recv_arr[1]
        status=readnumber()
        print("status = ", status)
        print("j_recv = ", j_recv)
        if(status<(sz_path-1)): #status - number elem,ent of array, which is number of last success crossroad
            num_path=0
            print("j_recv = ", j_recv)
            for i in range(0, (status-1)):
                num_path=num_path+robot_action[i].counter #=6 on othod to second crossroad
            num_path=num_path+j_recv
            print("num_path = ", num_path)
            print("path[num]=", path[num_path])
            print("barrier")
            break
        if(status==(sz_path-1)):
            print("success")
            break
        time.sleep(0.1)
        
#sudo i2cdetect -y 1
