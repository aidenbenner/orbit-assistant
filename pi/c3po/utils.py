import json


def format_data(payload):
    """
    dehydrate data into json string then encode to ascii
    """
    final = (json.dumps(payload) + '\n').encode('ascii')
    return final
