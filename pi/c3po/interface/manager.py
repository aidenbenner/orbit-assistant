from threading import Thread
from serial import Serial
from utils import parse_input


class SerialManager(Thread):
    """
    Thread that listens and writes to Serial
    """

    def __init__(self, name, valid_events):
        Thread.__init__(self)
        self.name = name
        self.valid_events = valid_events
        self.serial = Serial('/dev/ttyACM0')

    def run(self):
        s = self.serial

        while True:
            # If no buffer in serial, skip the rest
            if s.in_waiting == 0:
                continue

            line = s.readline()
            try:
                event, payload = parse_input(line, self.valid_events)
            else:
                # emit the event
                pass
            finally:
                pass
