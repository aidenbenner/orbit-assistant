"""
This module is responsible for fetching info according to event then write to
serial
"""
from datetime import datetime
from c3po import config, api
from c3po.utils import format_data


def fetch(event_name, options):
    """
    Determine what operation to perform according to event_name
    """
    if event_name == config.GET_NAME:
        return config.name
    elif event_name == config.GET_TIME:
        return datetime.now().strftime('%I:%M')
    elif event_name == config.GET_WEATHER:
        return api.get_weather(config.city, config.country)


def handle(event_name, options, serial):
    """
    Get info then write to serial
    """
    payload = fetch(event_name, options)
    event = event_name.split('_')[-1]
    print(event)
    print(payload)
    serial.write(format_data(event, payload))
