# c3po  

By Aiden Benner and Lawrence Pan

## Developer setup

```bash
$ # clone this repo
$ git clone https://github.com/aidenbenner/spy-hunter
$ # Install vim from your package manager
$ sudo apt-get install vim
$ # Install platformIO CLI
$ sudo pip install -U platformio
$ # Init project & install the OLED driver
$ cd spy-hunter
$ ./init.sh
$ # To upload your code to the board, do
$ platformio run
```

## Style

##### Code formatting

* 2 space indentation
* space before brackets before all function calls and definitions before ifs and for loops
* curly brace on new line

##### Naming

* structs - camel case
* everything else - underscores
* functions - verbs
* variables - nouns

## Design
