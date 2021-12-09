export function api_key_param() {
    return "?API_KEY=" + process.env.MIX_API_KEY_MD5;
}

// *****************************************************************************

export function api_url(path, params = {}) {
    // let url = '/api/v1/' + path + api_key_param();  // add API key to the url
    // let url = path + api_key_param();  // add API key to the url
    let url = path;

    // add parameters to the url
    let first = true;
    for (const [key, value] of Object.entries(params)) {
        let prefix = first ? '?' : '&';
        url += `${prefix}${key}=${value}`;      // &key1=param1&key2=param2...
        if (first) first = false;
    }
    return url;
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

// *****************************************************************************

export class Api {
    static get(url, params) {
        return axios.get(api_url(url, params))
    }
    
    static post(url, params, data) {
        return axios.post(api_url(url, params), data)
    }
}
