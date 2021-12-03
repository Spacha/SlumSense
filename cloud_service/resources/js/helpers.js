export function datetime(datetime) {
    return moment(datetime).format('YYYY-MM-DD HH:mm:ss')
}

export function temperature(temperature, precision = 2) {
    return `${temperature.toFixed(precision)} °C`;
}

export function pressure(pressure, precision = 2) {
    return `${pressure.toFixed(precision)} hPa`;
}

export function humidity(humidity, precision = 2) {
    return `${humidity.toFixed(precision)} %`;
}

