# Cloud Service

Contains the project files and code fot the cloud service software. It runs in a Linux Ubuntu server (AWS EC2).

Its main purpose is to offer a REST API for the gateway(s) to use for saving sensor data. The server stores the data to a MySQL database and also offers a browser-based user interface for monitoring the devices and analyzing the data.

* PHP8/Laravel 8
	* MySQL
	* REST API
		* Authentication via API key
* Interactive Vue.js front-end
* Plotting library?

## Installation to the server

1. Add a new subdomain slum.example.org
2. Configure the subdomain (path: `~/slum.example.org/cloud_service`)
	* also add SSH certificates and other generic stuff
4. Sparse checkout the repository
```shell
cd ~
git clone --no-checkout git@github.com:Spacha/SlumSense # clone without checkout
mv SlumSense slum.example.org				# rename the folder (for nginx)
cd slum.example.org
git config core.sparseCheckout true			# enable sparse checkout
echo "cloud_service"> .git/info/sparse-checkout		# add only this folder to the checkout
git checkout master					# finally checkout
```
5. Laravel setup
```shell
cp .env.example
nano .env						# update to match the server
composer update
php artisan key:generate				# application key
php artisan migrate					# DB migrations
npm i							# front-end
npm run prod
```
