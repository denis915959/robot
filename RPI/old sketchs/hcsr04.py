#ultrazvuk + servo
import RPi.GPIO as GPIO
import time
 
#GPIO Mode (BOARD / BCM)
GPIO.setmode(GPIO.BCM)
 
#set GPIO Pins
GPIO_TRIGGER_LEFT = 27  # GPIO27 (1-27)
GPIO_ECHO_LEFT = 17     # GPIO17 (1-27)
GPIO_TRIGGER_RIGHT = 24 # GPIO24 (1-27)
GPIO_ECHO_RIGHT = 23    # GPIO23 (1-27)
 
#set GPIO direction (IN / OUT)
GPIO.setup(GPIO_TRIGGER_LEFT, GPIO.OUT)  #OUT
GPIO.setup(GPIO_ECHO_LEFT, GPIO.IN)
GPIO.setup(GPIO_TRIGGER_RIGHT, GPIO.OUT)  #OUT
GPIO.setup(GPIO_ECHO_RIGHT, GPIO.IN)

def distance_left():
    # set Trigger to HIGH
    GPIO.output(GPIO_TRIGGER_LEFT, True)
 
    # set Trigger after 0.01ms to LOW
    time.sleep(0.00001)
    GPIO.output(GPIO_TRIGGER_LEFT, False)
    
    StartTime = time.time()
    StopTime = time.time()
 
    # save StartTime
    """while(1):
        print(GPIO.input(GPIO_ECHO_LEFT))
        time.sleep(0.5)"""
    while GPIO.input(GPIO_ECHO_LEFT) == 0:  #ghj,ktvf pltcm!
        StartTime = time.time()
    
 
    # save time of arrival
    while GPIO.input(GPIO_ECHO_LEFT) == 1:
        #print(1)
        StopTime = time.time()
 
    # time difference between start and arrival
    TimeElapsed = StopTime - StartTime
    # multiply with the sonic speed (34300 cm/s)
    # and divide by 2, because there and back
    distance = (TimeElapsed * 34300) / 2
 
    return distance
    
    
def distance_right():
    # set Trigger to HIGH
    GPIO.output(GPIO_TRIGGER_RIGHT, True)
 
    # set Trigger after 0.01ms to LOW
    time.sleep(0.00001)
    GPIO.output(GPIO_TRIGGER_RIGHT, False)
 
    StartTime = time.time()
    StopTime = time.time()
 
    # save StartTime
    """while(1):
        print(GPIO.input(GPIO_ECHO_LEFT))
        time.sleep(0.5)"""
    while GPIO.input(GPIO_ECHO_RIGHT) == 0:  #ghj,ktvf pltcm!
        StartTime = time.time()
    
 
    # save time of arrival
    while GPIO.input(GPIO_ECHO_RIGHT) == 1:
        #print(1)
        StopTime = time.time()
 
    # time difference between start and arrival
    TimeElapsed = StopTime - StartTime
    # multiply with the sonic speed (34300 cm/s)
    # and divide by 2, because there and back
    distance = (TimeElapsed * 34300) / 2
 
    return distance
    
    
def exact_distance_left():
    dist1 = distance_left()
    dist2 = distance_left()
    dist3 = distance_left()
    dist_left=(dist1+dist2+dist3)/3
    return(dist_left)
    
    
def exact_distance_right():
    dist1 = distance_right()
    dist2 = distance_right()
    dist3 = distance_right()
    dist_right=(dist1+dist2+dist3)/3
    return(dist_right)
    

import time
from adafruit_servokit import ServoKit

# Set up the PCA9685 board
kit = ServoKit(channels=16)
kit.frequency = 50  # Set the PWM frequency to 50 Hz

coefficient_angle = 130/90
start_left_angle = 20
start_right_angle = 160
min_step = 6 

# Define the functions to set the angle of the servos
def set_left_angle(angle):
    if((angle>180)or(angle<0)):
        return(False)
    else:
        kit.servo[14].angle = angle # number of pin in pca9685
        return(True)
    

def set_right_angle(angle):
    if((angle>180)or(angle<0)):
        return(False)
    else:
        kit.servo[15].angle = angle
        return(True)
 

 
if __name__ == '__main__':
    try:
        while True:
            step = 6 # min step!!!
            #set_left_angle(start_left_angle)
            k=start_right_angle//min_step
            right_angle = start_right_angle
            left_angle = start_left_angle
            for i in range(0, k):
                right_angle = right_angle - min_step
                left_angle = left_angle + min_step
                set_right_angle(right_angle)
                set_left_angle(left_angle)
                time.sleep(0.4)
                print("left distance = ", exact_distance_left())
                print("right distance = ", exact_distance_right())
                
            """time.sleep(1)
            dist1 = distance_left()
            dist2 = distance_left()
            dist3 = distance_left()
            dist_left=(dist1+dist2+dist3)/3
            print ("Measured Distance left = %.1f cm" % dist_left)
            dist1 = distance_right()
            dist2 = distance_right()
            dist3 = distance_right()
            dist_right=(dist1+dist2+dist3)/3
            print ("Measured Distance right = %.1f cm" % dist_right)
            time.sleep(1)"""
            """set_left_angle(100)
            time.sleep(2)"""
 
        # Reset by pressing CTRL + C
    except KeyboardInterrupt:
        print("Measurement stopped by User")
        GPIO.cleanup()








"""import RPi.GPIO as GPIO
import time

# Define GPIO pins
trig_pin = 19
echo_pin = 26

# Set GPIO mode and pins
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(trig_pin, GPIO.OUT)
GPIO.setup(echo_pin, GPIO.IN)

# Define function to measure distance
def measure_distance():
    # Send 10us pulse to trigger pin
    GPIO.output(trig_pin, True)
    time.sleep(0.00001)
    GPIO.output(trig_pin, False)

    # Wait for echo pin to go high
    print(1)#beskonechny cikl
    while GPIO.input(echo_pin) == 0: #0
        pulse_start = time.time()

    print(1)
    # Wait for echo pin to go low
    while GPIO.input(echo_pin) == 1:
        pulse_end = time.time()
    print(1)

    # Calculate distance in cm
    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150
    distance = round(distance, 2)

    return distance

# Read distance values and print them
while True:
    distance = measure_distance()
    print("Distance: {} cm".format(distance))
    time.sleep(1)

# Clean up GPIO pins
GPIO.cleanup()"""
