## Gateway

Contains the project files and code fot the gateway software. The gateway runs on Linux Raspbian (Raspberry Pi 3B+).

It is a python client that asks (or just reads) data from the BLE peripheral and sends it to the cloud using a REST API.

* Latest Raspbian OS
	* Automatic WiFi connectivity (or ethernet)
* Python 3.9
	* [Bleak](https://github.com/hbldh/bleak) for BLE

## Installation

Upload to the server via `scp` and set the API key.

On local machine, in the same network with the gateway device:
```shell
$ cd gateway
$ scp main.py pi@192.168.1.141:~/gateway
```

On the gateway device:
```shell
$ export SLUMWAY_API_KEY=MyAPIKeyDefinedInCloudSrvr
```
