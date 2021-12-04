export function api_key_param() {
    return "?API_KEY=" + process.env.MIX_API_KEY_MD5;
}

// *****************************************************************************

export function api_url(path) {
    return '/api/v1/' + path + api_key_param();
}

export class Format
{
    static datetime(datetime) {
        return moment(datetime).format('YYYY-MM-DD HH:mm:ss');
    }

    static temperature(temperature, precision = 2) {
        return `${temperature.toFixed(precision)} °C`;
    }

    static pressure(pressure, precision = 2) {
        return `${pressure.toFixed(precision)} hPa`;
    }

    static humidity(humidity, precision = 2) {
        return `${humidity.toFixed(precision)} %`;
    }
}

