from itertools import cycle
import time

scheduler = [
	[1, 255, 0, 0, 0.5],
	[1, 0, 255, 0, 0.5],
	[1, 0, 0, 255, 0.5],
	[1, 255, 0, 255, 0.5],
	[1, 0, 255, 255, 0.5],
	[1, 255, 255, 0, 0.5],
]

def publish(mode, r, g, b):
	f = open("string.txt", "w")
	f.write(mode + "," + r + "," + g + "," + b)


while True:
	for schedule in scheduler:
		mode = str(schedule[0])
		r = str(schedule[1])
		g = str(schedule[2])
		b = str(schedule[3])
		interval = schedule[4]
		
		publish(mode, r, g, b)
		time.sleep(interval)
