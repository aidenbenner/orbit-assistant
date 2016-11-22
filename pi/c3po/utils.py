import json


def format_data(event_name, payload):
    """
    dehydrate data into json string then encode to ascii
    """
    data = {'event': event_name, 'payload': payload}
    return json.dumps(data).encode('ascii') + '\r\n'
