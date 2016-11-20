# Backend for C-3PO
**Note:** The backend will eventually be deployed on a Raspberry PI but it also
works on a x86 PC.

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
```
