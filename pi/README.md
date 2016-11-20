# Backend for C-3PO
**Note:** The backend will eventually be deployed on a Raspberry PI but it also
works on a x86 PC.

## API

#### Events

* Sending events from Tiva to PI
  - Name has to be **uppercase**.
  - VERB_NOUN (eg. GET_NAME, GET_WEATHER, GET_JOKES).
  - Supply **exactly** one option, supply a trivial option `NULL` when necessary
  - End with a newline char `\r\n`
  - eg. `Serial.println('GET_NAME:NULL')`

* Receiving events from PI
  - Pi will send json string with two properties
    * event
    * payload
  - the event name is simply the `noun`.
  - Example response to `GET_NAME:NULL`:
```json
'{
  "event": "NAME",
  "payload": "Lawrence Pan"
}'
```

### Responsibilities

* Communicate via serial port
* Fetch data
* Web server (settings)

### Developer setup

Make sure you have the following installed
* Python3
* pip
* virtualenv

Run
```bash
cd pi/
virtualenv -p python3 venv
source ./venv/bin/activate
pip install -r requirements.txt
python entry.py # Start the serial manager
```
