<template>
    <div>
        <h4>Measurements</h4>
        <div class="mb-2">
            <a class="btn btn-secondary" v-on:click="getMeasurements"><i class="fa fa-sync"></i></a>
            <a href="/measurements" class="btn btn-primary">All measurements</a>
        </div>
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
const MAX_MEASUREMENTS = 50;

export default {
    data: () => ({
        measurements: []
    }),
    mounted() {
        console.log('Component mounted.');
        this.getMeasurements();
        /*
        Echo.channel('measurements')
            .listen('MeasurementStored', (e) => {
                console.log(e.measurement);
                this.measurements.push(e.measurement);
        });
        */
    },
    methods: {
        getMeasurements()
        {
            var mainContext = this
            axios.get(api_url(`measurements/${MAX_MEASUREMENTS}`))
            .then(function (response) {
                // handle success
                mainContext.measurements = response.data;
            })
            .catch(function (error) {
                // handle error
                console.log(error);
            });
        }
    }
}
</script>
