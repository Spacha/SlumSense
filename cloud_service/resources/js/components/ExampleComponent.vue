<template>
    <div>
        <h4>Measurements</h4>
        <a href="/measurements" class="btn btn-primary mb-2">All measurements</a>
        <table class="table table-bordered">
            <thead>
                <tr>
                    <th scope="col">Date & time</th>
                    <th scope="col">Temperature</th>
                    <th scope="col">Pressure</th>
                    <th scope="col">Humidity</th>
                </tr>
            </thead>
            <tbody v-if="measurements.length > 0">
                <tr v-for="measurement in measurements.slice().reverse()">
                    <td>{{ $datetime(measurement.created_at) }}</td>
                    <td>{{ $temperature(measurement.temperature) }}</td>
                    <td>{{ $pressure(measurement.pressure) }}</td>
                    <td>{{ $humidity(measurement.humidity) }}</td>
                </tr>
            </tbody>
            <tbody v-else>
                <tr>
                    <td colspan="4">No measurements.</td>
                </tr>
            </tbody>
        </table>
    </div>
</template>

<script>
export default {
    data: () => ({
        measurements: []
    }),
    mounted() {
        var mainContext = this
        axios.get('/api/v1/measurements?API_KEY=749e1a86cad252aacf2c05f324e7d72a')
            .then(function (response) {
                // handle success
                console.log(response);
                mainContext.measurements = response.data
            })
            .catch(function (error) {
                // handle error
                console.log(error);
            });
        console.log('Component mounted.')
        /*
        Echo.channel('measurements')
            .listen('MeasurementStored', (e) => {
                console.log(e.measurement);
                this.measurements.push(e.measurement);
        });
        */
    }
}
</script>
