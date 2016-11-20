from threading import Thread
from serial import Serial
from c3po.handler import handle


def parse_input(line, valid_events):
    """
    Parse serial input sent from Tiva
    Raise error if event not valid
    return event_type and payload
    """
    event, payload = line.decode('utf8').rstrip('\r\n').split(':')
    if event not in valid_events:
        raise Exception("Invalid event!")
    return event, payload


class SerialManager(Thread):
    """
    Thread that listens and writes to Serial
    """

    def __init__(self, name, valid_events):
        Thread.__init__(self)
        self.name = name
        self.valid_events = valid_events
        self.serial = Serial('/dev/ttyACM1')

    def run(self):
        s = self.serial

        while True:
            # If no buffer in serial, skip the rest
            if s.in_waiting == 0:
                continue

            line = s.readline()
            try:
                event, options = parse_input(line, self.valid_events)
            except Exception as e:
                print(line)
                print(e)
                continue
            handle(event, options, self.serial)
