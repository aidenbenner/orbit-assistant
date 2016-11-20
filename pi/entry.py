from c3po import SerialManager, config
import sys

serial_port = '//dev/ttyACM0'
if len(sys.argv) > 1 :
    serial_port = sys.argv[1] 
    print('Using serial port provided')
    print(sys.argv[1])
    
sm = SerialManager('Serial', config.valid_events,
                        serial_port)
sm.start()
