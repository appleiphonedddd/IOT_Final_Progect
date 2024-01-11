# NDHU_IOT_Project


`NDHU_IOT` project belongs to the Intelligent IoT technologies and applications at National Dong Hwa University. The purpose of this project is by scanning the barcode scanner, and if the authentication is correct, the relay can be enabled and disabled.

## Contents

- [NDHU\_IOT\_Project](#ndhu_iot_project)
  - [Contents](#contents)
    - [Getting Started](#getting-started)
          - [Requirements](#requirements)
          - [Installation](#installation)
    - [Directory Structure](#directory-structure)
    - [Deployment](#deployment)
    - [Frameworks Used](#frameworks-used)
    - [Author](#author)


### Getting Started

###### Requirements

1. Arduino IDE 1.8.19
2. Arduino Uno
3. ESP8266 NodeMCU
4. Relay
5. 4 X 4 Keypad
6. Barcode scanner
7. MySQL
8. phpMyAdmin
9. LCD
10. Windows 10
11. PHP

###### Installation

1. Clone the repo and enter repo folder

```sh
git clone https://github.com/appleiphonedddd/NDHU_IOT_Final_Project_2023.git
cd NDHU_IOT_Final_Project_2023/System
```

2. Install `requirement.txt`library in Arduino IDE

3. Install [Apache](https://www.apachelounge.com/download/) and [phpMyAdmin](https://www.phpmyadmin.net/downloads/)and [MySQL](https://dev.mysql.com/downloads/installer/) and [PHP](https://www.php.net/releases/index.php) in Windows 10


### Directory Structure

```
filetree 
├── README.md
├── System
│   ├── esp8266final.ino
│   ├── requirement.txt
│   └── SCANNER_arduino.ino
└── Web
    ├── check.php
    ├── config.php
    ├── index.php
    ├── personlogin.php
    └── s_member.php
```

### Deployment

1. Enter Apache24 dictionary then check Apache service is start
```sh
cd C:\Apache24\bin
net start Apache2.4
```

2. Add php related modules and settings to httpd.conf

3. Modify the path pointed by localhost to the place where you want to store the website

4. Add personal information in MySQL by phpMyAdmin

5. Execute `.ino` on Arduino IDE


### Frameworks Used

- [MySQL](https://www.mysql.com/)
- [phpMyAdmin](https://www.phpmyadmin.net/)
- [Apache](https://httpd.apache.org/)
- [PHP](https://www.php.net/)

### Author

zozo5120@outlook.com

Name:Egor Lee

611221220@gms.ndhu.edu.tw

Name:Yellow Yu

611221246@gms.ndhu.edu.tw

Name:HUANG JUN TING
