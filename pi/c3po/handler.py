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
        return datetime.now().strftime('%Y/%m/%d:%H:%M:%S')
    elif event_name == config.GET_WEATHER:
        return api.get_weather(options)
    elif event_name == config.GET_NEWS:
        return api.get_news(int(options))
    elif event_name == config.PRINT_DEBUG:
        return options


def handle(event_name, options, serial):
    """
    Get info then write to serial
    """
    payload = fetch(event_name, options)
    serial.write(format_data(payload))
