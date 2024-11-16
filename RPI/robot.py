import L298D
import LM393
import time 

time.sleep(4)
lm393 = LM393.LM393()
motor = L298D.Motor()
lm393.start_with_delay()
motor.rotate_right(200, 1.5)
print("left = ", lm393.get_speed_left())
print("right = ", lm393.get_speed_right())
motor.close() 
