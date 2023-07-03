# Рабочая версия кода для Распберри с запуском робота! в дальнейшем использовать этот код!
import time
import random #generate id of client
from paho.mqtt import client as mqtt_client
from operator import length_hint

import smbus
import os
import copy
bus = smbus.SMBus(1)
SLAVE_ADDRESS = 0x20

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


def mode_1(robot_action):
	sz_path=len(robot_action)
	result = [] # результат работы mode1
	print("robot_action      ", robot_action)
	
	writenumber(1) # this is mode!
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
			result.append(num_path)   #path[num_path])
			result.append(num_path+1)   #path[num_path + 1])
			print("num_path = ", num_path)
			#print("path[num]=", path[num_path])
			print("barrier")
			break
		if(status==(sz_path-1)):
			print("success")
			result.append(1)
			result.append(-1) #path[len(path)-1])
			result.append(-1) #path[len(path)-1])
			break
		time.sleep(0.1)
	return(result)

def robot_trip(robot_action):
	result = [] # результат работы robot_trip
	result = mode_1(robot_action)
	if(result[0]==0):
		print("barrier!")
		#вызов result[0]=mode_2()
	if(result[0]==1):
		print("success!")
	return(result)


        
robot_action=[]
#path=[]
#path=[0, 2, 1, 2, 0] #вместо этой строки path должен строиться сам
action_tmp=-1           #принятое значение поля action, которое еще не добавлено в robot_action
counter_tmp=-1          #принятое значение поля counter, которое еще не добавлено в robot_action
recv_counter=0          #счетчик приятых сообщений при приеме массива robot_action
start_recv=False        #прием массива robot_action начался?
actions_size=-1         #размер массива robot_action
array_is_received=False #массив robot_action принят целиком?

broker = 'broker.emqx.io'
port = 1883
topic = "python/mqtt"
username = 'emqx'
password = 'public'

# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 100)}'

def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %dn", rc)

    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def on_message(client, userdata, msg):
    global action_tmp
    global counter_tmp
    global recv_counter #counter of received messages
    global start_recv
    global actions_size
    global robot_action
    global array_is_received
            
    print("counter = ", recv_counter) #good work!

    message=int(msg.payload.decode())
        
    print(f"Received `{message}` from `{msg.topic}` topic")
    if(len(robot_action)<actions_size): # action_counter < actions_size):
        print("in if !!!!!!!")
        if(recv_counter>=2): #receive of array
            if(recv_counter%2==0):
                action_tmp=message
            else:
                counter_tmp=message
                robot_action.append(robot_path_node(action_tmp, counter_tmp))
        
    if(recv_counter==1):
            #print("start -1")
            actions_size=message

    if(message==-1):
        start_recv=True
            
    if(start_recv==True):
        print("clear")
        start_recv=False
        robot_action.clear()
        recv_counter=0 
        action_tmp=-1
        counter_tmp=-1
        array_is_received = False
            
    recv_counter+=1
    print("len before print = ", len(robot_action))
    if((len(robot_action)==actions_size)and(array_is_received==False)): # array was receive and array send to arduino!
        array_is_received=True


def mqtt_communication(): # может вызываться неограничеенное чсло раз
    global array_is_received
    global robot_action
    #global path
    client = connect_mqtt()
    client.loop_start()
    client.subscribe(topic)
    while(array_is_received==False):
        client.on_message=on_message
    
    client.loop_stop()
    client = connect_mqtt()
    client.loop_start()
    
    client.publish(topic, "-2") #прием прошел успешно, уведомляем об этом склад
    for i in range(0, len(robot_action)):
        print(robot_action[i].action, "  ", robot_action[i].counter)
        
    #вместо этого блока должна быть функция robot_trip
    print(" ") #robot go!
    #time.sleep(5)
    result_trip=robot_trip(robot_action)
    print("robot is work!")
    
    #result_trip=[1, -1, -1]
    
    # результат поездки робота
    answer=str(result_trip[0])+" " + str(result_trip[1]) + " " + str(result_trip[2]) + " " #пробел в конце обязателен, иначе распарсер на складе не будет работать!
    client.publish(topic, answer) # 1 - робот успешно взял товар, 0 - встретил препятствие, которое робот не может объехать
    time.sleep(0.4)
    client.publish(topic, answer) 
    time.sleep(0.4)
    client.publish(topic, answer)
    
    
    global action_tmp
    global counter_tmp
    global recv_counter
    global start_recv
    global actions_size
    
    #возвращаем значения переменных в исходное состояние (для повторного вызова функции mqtt_communication)
    robot_action.clear()
    action_tmp=-1
    counter_tmp=-1
    recv_counter=0
    start_recv=False
    actions_size=-1
    array_is_received=False
    
    client.loop_stop() 
    
    
for i in range(0, 1):
    mqtt_communication()


