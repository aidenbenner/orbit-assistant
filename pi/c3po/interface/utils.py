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
