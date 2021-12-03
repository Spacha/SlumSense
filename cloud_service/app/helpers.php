<?php

function temperature($temperature, $precision = 2) : string
{
	$temperature = number_format($temperature, $precision);
	return "{$temperature} °C";
}

function pressure($pressure, $precision = 2) : string
{
	$pressure = number_format($pressure, $precision);
	return "{$pressure} hPa";
}

function humidity($humidity, $precision = 2) : string
{
	$humidity = number_format($humidity, $precision);
	return "{$humidity} %";
}
