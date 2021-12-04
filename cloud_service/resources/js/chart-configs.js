function formatLabel(context, callback) {
    let label = context.dataset.label || '';

    if (label) {
        label += ': ';
    }
    if (context.parsed.y !== null) {
    	label += callback(context.parsed.y)
    }
    return label;
}


/*********************************************
 * Temperature chart configurations.
 *********************************************/
export const tempChartConfig = {
	data: {
		labels: [],
		datasets: [{
			borderWidth: 1,
			pointRadius: 2,
			label: 'Temperature (°C)',
			backgroundColor: 'rgb(255, 99, 132)',
			borderColor: 'rgb(255, 99, 132)',
			data: [],
			type: 'line',
			order: 0
		}]
	},
	options: {
		maintainAspectRatio: false,
		plugins: {
			title: {
				display: false,
				text: 'Sensor data from last X (h)',
			},
			legend: {
				display: true
			},
			tooltip: {
                callbacks: {
                	label: (context) => formatLabel(context, format.temperature)
                }
            }
		},

		scales: {
			x: {
				type: 'time',
				time: {
					unit: 'hour'
				}
			},
			y: {
				type: 'linear',
				display: true,
				position: 'left',
				min: -5,
				max: 35
			}
		}
	}
}

/*********************************************
 * Pressure chart configurations.
 *********************************************/
export const presChartConfig = {
	data: {
		labels: [],
		datasets: [{
			borderWidth: 1,
			pointRadius: 2,
			label: 'Pressure (hPa)',
			backgroundColor: 'rgb(254, 217, 131)',
			borderColor: 'rgb(254, 217, 131)',
			data: [],
			type: 'line',
			order: 0
		}]
	},
	options: {
		maintainAspectRatio: false,
		plugins: {
			title: {
				display: false,
				text: 'Sensor data from last X (h)',
			},
			legend: {
				display: true
			},
			tooltip: {
                callbacks: {
                	label: (context) => formatLabel(context, format.pressure)
                }
            }
		},

		scales: {
			x: {
				type: 'time',
				time: {
					unit: 'hour'
				}
			},
			y: {
				type: 'linear',
				display: true,
				position: 'left',
				min: 950,
				max: 1050
			}
		}
	}
}


/*********************************************
 * Humidity chart configurations.
 *********************************************/
export const humiChartConfig = {
	data: {
		labels: [],
		datasets: [{
			borderWidth: 1,
			pointRadius: 2,
			label: 'Humidity (%)',
			backgroundColor: 'rgb(152, 201, 238)',
			borderColor: 'rgb(152, 201, 238)',
			data: [],
			type: 'line',
			order: 0
		}]
	},
	options: {
		maintainAspectRatio: false,
		plugins: {
			title: {
				display: false,
				text: 'Sensor data from last X (h)',
			},
			legend: {
				display: true
			},
			tooltip: {
                callbacks: {
                	label: (context) => formatLabel(context, format.humidity)
                }
            }
		},

		scales: {
			x: {
				type: 'time',
				time: {
					unit: 'hour'
				}
			},
			y: {
				type: 'linear',
				display: true,
				position: 'left',
				// min: 0.0,
				// max: 100.0
			}
		}
	}
}