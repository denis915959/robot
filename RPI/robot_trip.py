# after copy into mqtt2!
import smbus
import time
import os
import copy
#command=1
bus = smbus.SMBus(1)
SLAVE_ADDRESS = 0x20       #04
class robot_path_node:
	def __init__(self, action1, counter1):
		self.action=action1
		self.counter=counter1

def writenumber_i2c(num):
	bus.write_byte(SLAVE_ADDRESS, int(num))
def write_array(a, b, c, d):
	bus.write_i2c_block_data(SLAVE_ADDRESS, a, [b, c, d])
def readnumber():
	num=int(bus.read_byte(SLAVE_ADDRESS))   #|bus.read_byte(SLAVE_ADDRESS)#read_byte
	return num
    
def mode_1(robot_action, path): # mode1 == robot_trip?
	sz_path=len(robot_action)
	result = [] # результат работы mode1

	writenumber_i2c(sz_path) #send robot_action to robot
	for i in range(0, sz_path):
		writenumber_i2c(robot_action[i].action)
		writenumber_i2c(robot_action[i].counter)
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

			result.append(0)
			result.append(path[num_path])
			result.append(path[num_path + 1])
			print("num_path = ", num_path)
			print("path[num]=", path[num_path])
			print("barrier")
			break
		if(status==(sz_path-1)):
			print("success")
			result.append(1)
			result.append(-1)#path[len(path)-1])
			result.append(-1)#path[len(path)-1])
			break
		time.sleep(0.1)
	return(result)



mode=2

if(mode==1): # эта штука для тестов, mode1 ,будет вызываться в robot_trip
	robot_action=[]
	#вместо этого блока прием данных с компа
	robot_action.append(robot_path_node(6, 1))#1
	robot_action.append(robot_path_node(4, 1))
	robot_action.append(robot_path_node(12, 0))
	robot_action.append(robot_path_node(6, 0))
	robot_action.append(robot_path_node(5, 1))
	robot_action.append(robot_path_node(0, 1))
	path=[0, 2, 1, 2, 0]
	"""robot_action.append(robot_path_node(2, 2))
	robot_action.append(robot_path_node(6, 0))
	path=[3, 2, 1]"""

	# эти 2 строчки скопировать в robot_trip
	writenumber_i2c(mode)
	result = mode_1(copy.deepcopy(robot_action), path)
	print(result)
if(mode==2):
	writenumber_i2c(mode)
	# движение на объезд препятствие. Здесь вместо него таймер
	time.sleep(10)
	writenumber_i2c(1) # 1 - команда на захват линии с поворотом влево
	#writenumber_i2c(2) # 2 - команда на захват линии с поворотом вправо
        
#sudo i2cdetect -y 1
