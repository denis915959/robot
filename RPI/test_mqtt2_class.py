# python test_mqtt2_class.txt
# Рабочая версия кода для Распберри!!!

import time
import random # генерируем id для клиента
from paho.mqtt import client as mqtt_client
from operator import length_hint

class robot_path_node:
	def __init__(self, action1, counter1):
		self.action=action1
		self.counter=counter1
                
                
import smbus
import os
import copy
bus = smbus.SMBus(1)
SLAVE_ADDRESS = 0x20

class robot_path_node:
	def __init__(self, action1, counter1):
		self.action=action1
		self.counter=counter1
		
class robot_path_node:
	def __init__(self, action1, counter1):
		self.action=action1
		self.counter=counter1

class MQTT_RPi:
    def __init__(self):
        self.robot_action=[]

def writenumber_i2c(num):
	bus.write_byte(SLAVE_ADDRESS, int(num))
def write_array(a, b, c, d):
	bus.write_i2c_block_data(SLAVE_ADDRESS, a, [b, c, d])
def readnumber():
	num=int(bus.read_byte(SLAVE_ADDRESS))
	return num


def mode_1(robot_action):
	sz_path=len(robot_action)
	result = [] # результат работы mode1
	
	writenumber_i2c(1) # это mode!
	writenumber_i2c(sz_path) #отправка пазмера robot_action на робота
	for i in range(0, sz_path):
		writenumber_i2c(robot_action[i].action)
		writenumber_i2c(robot_action[i].counter)
		print(robot_action[i].action, "  ", robot_action[i].counter)

	status=25  #по умолчанию
	j_recv=-1
	number_recv_message=-1
	while(status==25): # опрос робота
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
        

class MQTT_RPi:
    def __init__(self):
        self.robot_action=[]
        self.array_is_received=False #массив robot_action принят целиком?
        self.broker = 'broker.emqx.io'
        self.port = 1883
        self.topic = "python/robot" #mqtt
        self.username = 'robot' # emqx
        self.password = 'public'
        self.client_id = f'python-mqtt-{random.randint(0, 100)}'
        self.connect_counter=0

    def connect_mqtt(self) -> mqtt_client:
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                print("Connected to MQTT Broker!")
                self.connect_counter=self.connect_counter+1
            else:
                #print("Failed to connect, return code %dn", rc)
                print("else")
        client = mqtt_client.Client(self.client_id)
        client.username_pw_set(self.username, self.password)
        client.on_connect = on_connect
        client.connect(self.broker, self.port)
        return client

    def on_message(self, client, userdata, msg):
        print("start on_message")
        action_tmp=0
        counter_tmp=0
        message=msg.payload.decode() #int(msg.payload.decode())
        print("k1")
        if((message[0]=="#")and(message[1]=="$")): #this is system messaage!
            print("err")
            return
        if((message[0]=="m")and(message[1]=="e")): #this is system messaage!
            print("err")
            return
        print("k2")
        message=str(message)
        len_message=len(message)
        if(len_message>2):
            print("clear")
            self.robot_action.clear()
            self.array_is_received = False
        i=0
        tmp=0
        print(f"Received `{message}` from `{msg.topic}` topic")
        while(message[i]!=","):
            tmp=tmp*10+int(message[i])
            i=i+1
        robot_action_sz=tmp
        i=i+1
        while(i < len_message):
            action_tmp=0
            while(message[i]!=","):
                action_tmp = action_tmp*10+int(message[i])
                i = i+1
            counter_tmp=0
            i = i+1
            while(message[i]!=","):
                counter_tmp = counter_tmp*10+int(message[i])
                i = i+1
            self.robot_action.append(robot_path_node(action_tmp, counter_tmp))
            i = i+1
        if(len(self.robot_action)==robot_action_sz): # Ни на что не влияет, вернуться, когда сделаю класс
            print("if")
            self.array_is_received=True

    def mqtt_communication(self): # может вызываться неограничеенное чсло раз self. не добавлены
        print("before connect")
        client = self.connect_mqtt()
        print("after connect")
        client.loop_start()
        print("after loop start")
        client.subscribe(self.topic)
        print("before while")
        while(self.array_is_received==False): # Ожидание подключения? FALSE в уонце сделать!
            client.on_message=self.on_message
            if(self.connect_counter==2):
                print("return")
                return(False)
            #print("in while") #при выводе на экран почему-то бесконечный цикл
        print("after while")
    
        client.loop_stop()
        print("after loop stop")
        client = self.connect_mqtt()
        print("after connect mqtt")
        client.loop_start()
        res = client.publish(self.topic, "-2") #прием прошел успешно, уведомляем об этом склад
        mode1_flag = False
        if(res[0]==0):
            print("Success send!")
            mode1_flag = True
        else:
            print("Not successfully send!")
        for i in range(0, len(self.robot_action)):
            print(self.robot_action[i].action, "  ", self.robot_action[i].counter)
        result_trip = []
        if(mode1_flag):
            result_trip = robot_trip(self.robot_action) 
            print("robot is work!")
        
        """#вместо этого блока должна быть функция robot_trip
        print(" ") #robot go!
        time.sleep(2)
        print("robot is work!")"""
    
        # n2 - количество усчпешно взятых товаров на момент отправки сообщения, n3==1 - препятствие слева, n4==1 - препятствие справа!!
        #result_trip=[1, -1, -1, -1, -1]   #номер элементов в path! 
        #result_trip=[0, 2, 3, -1, -1, 3, 4] # if 6

        #result_trip=[0, 2, 3, -1, -1]
        #result_trip=[0, 1, 2, -1, -1]

        #result_trip=[0, -1, 0, 1, 0]
        #result_trip=[1, 0, 1, -1, -1, 1, 2]
        # результат поездки робота
        #пробел в конце обязателен, иначе распарсер на складе не будет работать!
        mode = -2
        type_barrier = -1 # 1 - препятствие между двумя перекрестками, 2 - препятствие между ячейками
        if((result_trip[0]==0)and(result_trip[1]!=-1)):
            mode = 2
            type_barrier = 1
            print("type 1")
        if((result_trip[0]==0)and(result_trip[1]==-1)):
            mode = 2
            type_barrier = 2
        if((mode == 2)and(type_barrier == 1)):
            result_obiezd = [] # в этом месте вызов функции объезда препятствия! на выхрод массив из 2 элементов: 1) 1/0 - true/false 2)0/1/2 - нет поворота/влево/вправо
            if(result_obiezd==True):
                if(result_obiezd[1]>0): # т.е если удалось объехать препятствие
                    writenumber_i2c(mode)
                    time.sleep(1)
                    writenumber_i2c(result_obiezd[1]) # 1 - команда на захват линии с поворотом влево
                result_trip[0]=1
            else:
                result_trip[0]=0
        
        answer=""
        for i in range(0, len(result_trip)):
            answer = answer + str(result_trip[i])+" "
        client.publish(self.topic, answer) # 1 - робот успешно взял товар, 0 - встретил препятствие, которое робот не может объехать
        time.sleep(0.4)
        if(res[0]==0):
            print("Success send!")
        else:
            print("Not successfully send!")
        client.publish(self.topic, answer) 
        time.sleep(0.4)
        client.publish(self.topic, answer)
        print(answer)
    
        
    
        #возвращаем значения переменных в исходное состояние (для повторного вызова функции mqtt_communication)
        self.robot_action.clear()
        self.array_is_received=False
        client.loop_stop()
        return(True)
        
        
        
        
        
        
robot_action=[]
action_tmp=-1           #принятое значение поля action, которое еще не добавлено в robot_action
counter_tmp=-1          #принятое значение поля counter, которое еще не добавлено в robot_action
recv_counter=0          #счетчик приятых сообщений при приеме массива robot_action
start_recv=False        #прием массива robot_action начался?
actions_size=-1         #размер массива robot_action
array_is_received=False #массив robot_action принят целиком?


    
    
for i in range(0, 1):
    mqtt=MQTT_RPi()
    print("for")
    flag=mqtt.mqtt_communication()
    mqtt=None
    while(flag==False):
        time.sleep(1.5)
        print("while =False")
        """mqtt=MQTT_RPi()
        flag=mqtt.mqtt_communication()
        mqtt=None"""
        mqtt1=MQTT_RPi()
        flag=mqtt1.mqtt_communication()
        mqtt1=None


