#скопировать этот файл на распберри в выходные и робота тестировать! код по идее рабочий

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
	"""robot_action.append(robot_path_node(5, 1))#1
	robot_action.append(robot_path_node(4, 1))
	robot_action.append(robot_path_node(12, 0))
	robot_action.append(robot_path_node(6, 0))
	robot_action.append(robot_path_node(6, 1))
	robot_action.append(robot_path_node(0, 1))
	path=[0, 2, 3, 2, 0]"""
	robot_action.append(robot_path_node(2, 2))
	robot_action.append(robot_path_node(6, 0))
	path=[3, 2, 1]
	

	writenumber(mode)

	sz_path=len(robot_action)
	writenumber(sz_path) #send robot_action to robot
	for i in range(0, sz_path):
		writenumber(robot_action[i].action)
		writenumber(robot_action[i].counter)
		print(robot_action[i].action, "  ", robot_action[i].counter)


	status=25  #по умолчанию
	j_recv=-1
	number_recv_message=-1
	while(status==25):
		recv_message=readnumber()
		print("recv_message = ", recv_message)
		if(recv_message==255):
			number_recv_message=0
		if(number_recv_message==1):
			j_recv=recv_message
		if(number_recv_message==2):
			status=recv_message
			number_recv_message=-1
		if(number_recv_message!=-1):
			number_recv_message=(number_recv_message+1)%3

		"""len_m=len(str(recv_message)) # rasparser
		j_recv = recv_message[len_m-1]
		status=0
		for i in range(0, (len_m-1)):
			status = 10*status + int(string(recv_message[i]))"""

		if(status!=25):
			print("status = ", status)
			print("j_recv = ", j_recv)
		if(status<(sz_path-1)): #status - номер элемента массива robot_action, который был последним выполнен успешно
			num_path=0
			print("j_recv = ", j_recv)
			for i in range(0, status): #status-1
				num_path=num_path+robot_action[i].counter 
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
