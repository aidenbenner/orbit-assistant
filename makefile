all:
	pio run --target upload 

serial:
	pio run --target upload 
	pio device monitor
