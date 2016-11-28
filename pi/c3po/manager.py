from threading import Thread
from serial import Serial
from c3po.handler import handle
import time
import logging


def parse_input(line, valid_events):
    """
    Parse serial input sent from Tiva
    Raise error if event not valid
    return event_type and payload
    """
    payloads = line.decode('utf8').rstrip('\r\n').split(':')
    event = payloads[0]
    if event not in valid_events:
        raise Exception("Invalid event!")
    return event, payloads[1:]


class SerialManager(Thread):
    """
    Thread that listens and writes to Serial
    """

    def __init__(self, name, valid_events, serial_port):
        Thread.__init__(self)
        self.name = name
        self.valid_events = valid_events
        self.serial = Serial(serial_port)

    def run(self):
        s = self.serial
        print('Start polling on serial')

        while True:
            # If no buffer in serial, skip the rest
            if s.in_waiting == 0:
                continue

            line = s.readline()
            print('Input from tiva: ' + str(line))

            try:
                event, options = parse_input(line, self.valid_events)
            except Exception as e:
                logging.error(e)
                continue
            handle(event, options, self.serial)
