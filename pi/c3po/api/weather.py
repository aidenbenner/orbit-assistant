from requests import get

base_url = 'http://api.openweathermap.org/data/2.5/weather'
api_key = '910935e36577082d2e790ec4537ef07b'


def to_celcius(temp):
    """
    From Kelvin to Celcius
    """
    return temp - 273.15


def get_weather(city, country_code):
    """
    Get weather data from openweathermap.org
    return:
        - weather (eg. "Clear")
        - description (eg. "Clear sky")
        - temp (in Celcius, eg. 23)
        - temp_max (in Celcius, eg. 23)
        - temp_min (in Celcius, eg. 23)
        - pressure (in Torr, eg. 1018)
        - humidity (in %, eg. 89)
    """
    params = {'q': city + ',' + country_code, 'appid': api_key}
    result = get(base_url, params).json()

    return {'weather': result['weather'][0]['main'],
            'description': result['weather'][0]['description'],
            'temp': to_celcius(result['main']['temp']),
            'temp_max': to_celcius(result['main']['temp_max']),
            'temp_min': to_celcius(result['main']['temp_min']),
            'pressure': result['main']['pressure'],
            'humidity': result['main']['humidity']}
