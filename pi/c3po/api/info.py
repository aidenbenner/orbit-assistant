"""
Get and Set permanent informations
"""

import json


def get_info():
    with open('../../info.json') as info:
        data = json.load(info)

    return data


def set_info(options):
    """
    options:
        - attribute desired to be set (name, city, country)
        - value eg. Aiden
    """
    attr, value = options

    with open('../../info.json') as info:
        data = json.load(info)

    data[attr] = value
    return data
