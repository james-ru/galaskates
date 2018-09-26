import time

# Array of instructions:
# mode | red | green | blue | interval (until next instruction)
scheduler = [
	[1, 255, 0, 0, 0.5],
	[1, 0, 255, 0, 0.5],
	[1, 0, 0, 255, 0.5],
	[1, 255, 0, 255, 0.5],
	[1, 0, 255, 255, 0.5],
	[1, 255, 255, 0, 0.5],
]

# Edit string.txt with a new instruction
def publish(mode, r, g, b):
	f = open("string.txt", "w")
	f.write(mode + "," + r + "," + g + "," + b)

# Send off every instruction depending on their respective interval to publish()
while True:
	for schedule in scheduler:
		mode = str(schedule[0])
		r = str(schedule[1])
		g = str(schedule[2])
		b = str(schedule[3])
		interval = schedule[4]
		
		publish(mode, r, g, b)
		time.sleep(interval)
