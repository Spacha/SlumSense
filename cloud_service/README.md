# Cloud Service

Contains the project files and code fot the cloud service software. It runs in a Linux Ubuntu server (AWS EC2).

Its main purpose is to offer a REST API for the gateway(s) to use for saving sensor data. The server stores the data to a MySQL database and also offers a browser-based user interface for monitoring the devices and analyzing the data.

* PHP8/Laravel 8
	* MySQL
	* REST API
		* Authentication via API key
* Interactive Vue.js front-end
* Plotting library?
