"""
This module is responsible for fetching info according to event then write to
serial
"""
from datetime import datetime
from c3po import config, api
from c3po.utils import format_data


def format_date():
    now = datetime.now()
    return {'second': now.second,
            'minute': now.minute,
            'hour': now.hour,
            'day': now.day,
            'month': now.month,
            'year': now.year}


def fetch(event_name, options):
    """
    Determine what operation to perform according to event_name
    """
    if event_name == config.GET_INFO:
        return api.get_info()
    elif event_name == config.SET_INFO:
        return api.set_info(options)
    elif event_name == config.GET_DATE:
        return format_date()
    elif event_name == config.GET_WEATHER:
        return api.get_weather(options)
    elif event_name == config.GET_NEWS:
        return api.get_news(options)
    elif event_name == config.GET_MAILS:
        return api.get_mails(options)
    elif event_name == config.SEND_MAIL:
        return api.send_mail(options)
    elif event_name == config.PRINT_DEBUG:
        return options


def handle(event_name, options, serial):
    """
    Get info then write to serial
    """
    payload = fetch(event_name, options)
    serial.write(format_data(payload))
