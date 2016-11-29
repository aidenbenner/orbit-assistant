from c3po import SerialManager, config
import sys
import json
import os

serial_port = '/dev/ttyACM0'
filename = './info.json'

if len(sys.argv) > 1:
    serial_port = sys.argv[1]

if not os.path.exists(filename):
    # write default info to file
    with open(filename, 'w') as outfile:
        json.dump(config.info, outfile)

sm = SerialManager('Serial', config.valid_events, serial_port)
sm.start()
