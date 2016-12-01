# c3po
Portatble personal assitant based on Ti Tiva board.

## Features
* Display current time
* Display current weather depending on your location
* Reddit client
* Email client (read and reply to emails)
* Virtual keyboard
* Personal setting

## How does it work?
1. Two components: Raspberry PI and Ti Tiva.
2. A Python server that runs on the Raspberry PI listens to the Serial port
3. Ti Tiva prints `events` to Serial to request for information. eg. `GET_WEATHER:CA:Waterloo`
4. The Python server extracts the `event_name` and the `options`.
5. Depending on the two parameters mentioned above, the server does a certain series of operations.
6. If it is a `GET` event, python server will fetch data from the internet then send it back to Ti Tiva as a JSON string.
7. If it is an event with side effects (eg. `SET_NAME`, `SEND_MAIL`), the python server will simply return a status code.

### State management
* Inspired by the [redux](http://redux.js.org/) library.
* Single source of truth, read-only state
* See `state.cpp`

## Possible improvements
* Support asynchronous operations
  - Unfortunately, the board we are using does not support [multi-threading](http://energia.nu/guide/multitasking/). However, if we have a more powerful board, here is what we will do.
  - We supply an unique ID to the event we dispatch from Ti Tiva and then add the ID to a vector. eg. `GET_WEATHER:1fe3:CA:WATERLOO`
  - We dispatch a python `asyncio coroutine` to fetch the information
  - When we send the JSON string back to the Ti Tiva, we supply the `ID` as one of the fields.
  - On Ti Tiva, we have one thread constantly polling on the serial.
  - When it receives JSON, depending on the ID, we update the corresponding state variable (eg. `g_weather`)

## Challenges we ran into
* Parsing JSON
  - Hydrating JSON string on an embedded device is a little harder than just `JSON.parse` in JavaScript
* Working with the OLED library

By Aiden Benner and [Lawrence Pan](http://lpan.io)
