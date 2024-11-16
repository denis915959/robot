import search_width_class
import find_barriers
#import L298D
import LM393
import time

class Obiezd:
	def __init__(self, motor):
		self.m = motor
		self.lm393 = LM393.LM393()
		self.scaner = find_barriers.Lidar(self.m, self.lm393)
		self.search_width = search_width_class.Search_Width(self.m, self.lm393)
		
	def obiezd(self):
		width = self.scaner.calculate_barrier_width()
		if(width>19):
			self.m.go_front_to_distance(4, self.lm393) # 4 const sdelat
			time.sleep(0.5)
			result_obiezd = self.search_width.obiezd_left()
			return(result_obiezd)
"""		
test = Obiezd()
print("res = ", test.obiezd())
"""
