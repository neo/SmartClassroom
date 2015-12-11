from tweet import *
from arduino import *
import time
arduino = Arduino('COM6')
arduino.output([])

buttonPin = 8

print "INITIATE"
today = 0
total = 0

while True:
	button = arduino.getState(buttonPin)
	time.sleep(0.1)
	if button:
			today += 1
			total += 1
			tmp = arduino.getTemp()
			arduino.LCD(today, total, int(round(tmp)))
			name = str(raw_input("Who pressed me?! "))
			print tweet(name + " pressed the big red button!")
			time.sleep(1)