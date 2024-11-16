# python test_mqtt2.py
# Рабочая версия кода для Распберри!!!

import time
import random #generate id of client
from paho.mqtt import client as mqtt_client
from operator import length_hint

class robot_path_node:
	def __init__(self, action1, counter1):
		self.action=action1
		self.counter=counter1

class MQTT_RPi:
    def __init__(self):
        self.robot_action=[]
        
        
        
        
        
        
        
        
robot_action=[]
action_tmp=-1           #принятое значение поля action, которое еще не добавлено в robot_action
counter_tmp=-1          #принятое значение поля counter, которое еще не добавлено в robot_action
recv_counter=0          #счетчик приятых сообщений при приеме массива robot_action
start_recv=False        #прием массива robot_action начался?
actions_size=-1         #размер массива robot_action
array_is_received=False #массив robot_action принят целиком?

broker = 'broker.emqx.io'
#broker = 'broker.mosquitto.org'
port = 1883
topic = "python/robot" #mqtt
username = 'robot' # emqx
password = 'public'

# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 100)}'

def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            #print("Failed to connect, return code %dn", rc)
            print("else")
            

    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def on_message(client, userdata, msg):
    action_tmp=0
    counter_tmp=0
    #global recv_counter #counter of received messages
    global start_recv
    global actions_size
    global robot_action
    global array_is_received
            
    #print("counter = ", recv_counter) #good work!

    message=msg.payload.decode() #int(msg.payload.decode())
    if((message[0]=="#")and(message[1]=="$")): #this is system messaage!
        print("err")
        return
    if((message[0]=="m")and(message[1]=="e")): #this is system messaage!
        print("err")
        return
    message=str(message)
    len_message=len(message)
    if(len_message>2):
        print("clear")
        start_recv=False
        robot_action.clear()
        recv_counter=0 
        array_is_received = False
    i=0
    tmp=0
    print(f"Received `{message}` from `{msg.topic}` topic")

    while(message[i]!=","):
        tmp=tmp*10+int(message[i])
        i=i+1
    robot_action_sz=tmp
    print("robot_action_sz ", robot_action_sz)
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
        robot_action.append(robot_path_node(action_tmp, counter_tmp))
        i = i+1
    array_is_received=True


def mqtt_communication(): # может вызываться неограничеенное чсло раз
    global array_is_received
    global robot_action
    print("before connect")
    client = connect_mqtt()
    print("after connect")
    client.loop_start()
    print("after loop start")
    client.subscribe(topic)
    print("before while")
    while(array_is_received==False): # Ожидание подключения?
        client.on_message=on_message
        #print("in while") #при выводе на экран почему-то бесконечный цикл
    print("after while")
    
    client.loop_stop()
    print("after loop stop")
    client = connect_mqtt()
    print("after connect mqtt")
    client.loop_start()
    
    res = client.publish(topic, "-2") #прием прошел успешно, уведомляем об этом склад
    if(res[0]==0):
        print("Success send!")
    else:
        print("Not successfully send!")
    for i in range(0, len(robot_action)):
        print(robot_action[i].action, "  ", robot_action[i].counter)
        
    #вместо этого блока должна быть функция robot_trip
    print(" ") #robot go!
    time.sleep(2)
    print("robot is work!")
    
    # n2 - количество усчпешно взятых товаров на момент отправки сообщения, n3==1 - препятствие слева, n4==1 - препятствие справа!!
    result_trip=[1, -1, -1, -1, -1]   #   #номер элементов в path! 
    #result_trip=[0, 2, 3, -1, -1] # if 6

    #result_trip=[0, 1, 2, -1, -1]

    #result_trip=[0, -1, 0, 1, 0]
    #result_trip=[1, 0, 1, -1, -1]
#для mqtt_trip проверить, возвращается ли номер в path или сам path !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  
    # результат поездки робота
    #пробел в конце обязателен, иначе распарсер на складе не будет работать!
    answer=""
    for i in range(0, len(result_trip)):
        answer = answer + str(result_trip[i])+" "
    client.publish(topic, answer) # 1 - робот успешно взял товар, 0 - встретил препятствие, которое робот не может объехать
    time.sleep(0.4)
    if(res[0]==0):
        print("Success send!")
    else:
        print("Not successfully send!")
    client.publish(topic, answer) 
    time.sleep(0.4)
    client.publish(topic, answer)
    print(answer)
    
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


