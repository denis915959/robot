# ???????
import time
import random #generate id of client
from paho.mqtt import client as mqtt_client
from operator import length_hint

class robot_path_node:
	def __init__(self, action1, counter1):
		self.action=action1
		self.counter=counter1
        
robot_action=[]
action_tmp=-1
counter_tmp=-1  #counter of received messages
recv_counter=0
start_recv=False
actions_size=-1
array_is_received=False

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


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        global action_tmp
        global counter_tmp
        global recv_counter #counter of received messages
        global start_recv
        global actions_size
        global robot_action
        global array_is_received
        
        if(array_is_received==True):
            time.sleep(5) # funcion robot go!
            print("robot is work!")
            client.publish(topic, "1") # 1 - robot work success, 0 - barrier? which robot can't go around
            client.publish(topic, "-1") #number of success crossroad (-1 for success work!!)
            client.publish(topic, "-1")
            
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
                    #action_counter = action_counter + 1
        
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
        #print("len before print = ", len(robot_action))
        if((len(robot_action)==actions_size)and(array_is_received==False)): # array was receive and array send to arduino!
            array_is_received=True
            for i in range(0, actions_size):
                print(robot_action[i].action, "  ", robot_action[i].counter)
            client.subscribe(topic) #connect to stream!! 
            client.publish(topic, "-2") #receive is success!!
            """print(" ")
            time.sleep(5)
            print("robot is work!")
            client.publish(topic, "1") # 1 - robot work success, 0 - barrier? which robot can't go around
            client.publish(topic, "-1") #number of success crossroad (-1 for success work!!)
            client.publish(topic, "-1")"""
        """if(array_is_received==True):
            print(" ")
            time.sleep(5)
            print("robot is work!")
            client.publish(topic, "1") # 1 - robot work success, 0 - barrier? which robot can't go around
            client.publish(topic, "-1") #number of success crossroad (-1 for success work!!)
            client.publish(topic, "-1")"""
            
    """if(array_is_received==True):
        print(" ")
        time.sleep(5)
        print("robot is work!")
        client.publish(topic, "1") # 1 - robot work success, 0 - barrier? which robot can't go around
        client.publish(topic, "-1") #number of success crossroad (-1 for success work!!)
        client.publish(topic, "-1")"""
    client.subscribe(topic)
    client.on_message = on_message
    


def run():
    client = connect_mqtt()
    subscribe(client)
    """if(array_is_received==True):
        print(100)"""
    client.loop_forever()


if __name__ == '__main__':
    run()









"""import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("test/topic")

def on_message(client, userdata, msg):
    print(1)
    print(msg.topic+" "+str(msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message


client.connect("localhost", 1883, 60)

client.loop_start()
"""

