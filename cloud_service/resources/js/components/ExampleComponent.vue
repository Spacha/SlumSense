<template>
    <div>
        <!--
        <div class="mb-2">
            <a class="btn btn-secondary" v-on:click="getMeasurements"><i class="fa fa-sync"></i></a>
            <a href="/measurements" class="btn btn-primary">All measurements</a>
        </div>
        -->

        <div class="d-flex flex-column justify-content-center align-items-center border-bottom p-3">

            <div>
                <a class="btn btn-secondary" v-on:click="getMeasurements"><i class="fa fa-sync"></i></a>
                <div class="btn-group px-2">
                <!--<a class="btn btn-secondary" v-on:click="addRandomMeasurement"><i class="fa fa-plus"></i></a>-->
                    <a class="btn btn-primary"
                        v-for="period in periods"
                        :class="{ active: period.hours == currentPeriod.hours}"
                        v-on:click="changeMeasPeriod(period.hours)">
                        {{ period.display }}
                    </a>
                </div>
            </div>

            <div class="form-check form-switch mt-3">
                <input class="form-check-input" type="checkbox" id="noLimitCheckbox"
                    v-model="noResultLimit"
                    v-on:change="getMeasurements">
                <label class="form-check-label" for="noLimitCheckbox">No limit</label>
            </div>
        </div>

        <div class="card-body">

            <div>
                <line-chart ref="tempChart" :period="currentPeriod" :config="tempChartConfig" />
                <line-chart ref="presChart" :period="currentPeriod" :config="presChartConfig" />
                <line-chart ref="humiChart" :period="currentPeriod" :config="humiChartConfig" />
            </div>

            <table class="table table-bordered table-striped">
                <thead>
                    <tr>
                        <th scope="col">Date & time</th>
                        <th scope="col">Temperature</th>
                        <th scope="col">Pressure</th>
                        <th scope="col">Humidity</th>
                    </tr>
                </thead>
                <tbody v-if="measurements.length > 0">
                    <tr v-for="measurement in measurements">
                        <td>{{ $format.datetime(measurement.created_at) }}</td>
                        <td>{{ $format.temperature(measurement.temperature) }}</td>
                        <td>{{ $format.pressure(measurement.pressure) }}</td>
                        <td>{{ $format.humidity(measurement.humidity) }}</td>
                    </tr>
                </tbody>
                <tbody v-else>
                    <tr>
                        <td colspan="4">No measurements.</td>
                    </tr>
                </tbody>
            </table>
        </div><!-- card-body -->
    </div>
</template>

<script>
const MAX_MEASUREMENTS = 50;

import { tempChartConfig, presChartConfig, humiChartConfig } from '../chart-configs.js'

class Period {
    // period in hours
    constructor(hours, display, unit = 'hour') {
        this.hours = hours
        this.display = display
        this.unit = unit
    }
    getPeriod() {
        return [moment().subtract(this.hours, 'hours'), moment()]
    }
    get from() {
        return this.getPeriod()[0].unix();
    }
    get to() {
        return this.getPeriod()[1].unix();
    }
}

const periods = [
    new Period(1, '1h', 'minute'),
    new Period(6, '6h', 'hour'),
    new Period(24, '24h', 'hour'),
    new Period(7*24, 'Week', 'day')
]

export default {
    data: () => ({
        noResultLimit: false,
        periods: periods,
        currentPeriod: periods[0],
        measurements: [],
        tempChartConfig: tempChartConfig,
        presChartConfig: presChartConfig,
        humiChartConfig: humiChartConfig,
        chartRefs: []
    }),
    mounted() {
        this.chartRefs = [
            this.$refs.tempChart,
            this.$refs.presChart,
            this.$refs.humiChart
        ]

        this.getMeasurements();
        /*
        Echo.channel('measurements')
            .listen('MeasurementStored', (e) => {
                console.log(e.measurement);
                this.measurements.push(e.measurement);
        });
        */
    },
    computed: {
        // computes the measurements to chartable format
        // return 3 separate arrays: [temp, pres, humi]
        chartData() {
            var tempData = []
            var presData = []
            var humiData = []
            var timestamp = null

            this.measurements.forEach(m => {
                timestamp = moment(m.created_at);
                tempData.push({x: timestamp, y: m.temperature});
                presData.push({x: timestamp, y: m.pressure});
                humiData.push({x: timestamp, y: m.humidity});
            });

            return [tempData, presData, humiData];
        }
    },
    methods: {

        addRandomMeasurement() {
            this.addMeasurements([{
                x: new Date(),
                y: Math.random()*10
            }])
        },

        addMeasurements(tempData, presData, humiData, clear = false) {
            this.$refs.tempChart.addData(tempData, clear);
            this.$refs.presChart.addData(presData, clear);
            this.$refs.humiChart.addData(humiData, clear);
        },

        getMeasurements() {
            var mainContext = this;

            // set the time period
            let params = {
                from: this.currentPeriod.from,
                to: this.currentPeriod.to
            }

            // Read about softmax in the back-end (ApiController).
            if (!this.noResultLimit) {
                params.softmax = MAX_MEASUREMENTS
            }

            axios.get(api_url(`measurements`, params))
            .then(function (response) {
                // store the results & refresh the charts
                mainContext.measurements = response.data;
                mainContext.addMeasurements(...mainContext.chartData, true);
            })
            .catch(function (error) {
                // handle error (or not)
                console.log(error);
            });
        },

        changeMeasPeriod(hours) {
            this.currentPeriod = this.periods.find(period => period.hours == hours);
            
            // update the time axis units for each chart
            this.chartRefs.forEach(ref => {
                let options = ref.chart.config.options;
                options.scales.x.time.unit = this.currentPeriod.unit;
            });

            this.getMeasurements();
        }
    }
}
</script>
