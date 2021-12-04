<template>
<div class="h-100">
<canvas ref="chartCanvas" :height="height"></canvas>
</div>
</template>
<script>
import Chart from 'chart.js/auto';
//import { Chart, TimeScale } from 'chart.js';
import 'chartjs-adapter-moment';

// show last 24h by default

const labels = [
'20.4.', '21.4.', '22.4.', '23.4.', '24.4.', '25.4.', '26.4.', '27.4.', '28.4.', '29.4.', '30.4.', '1.5.', '2.5.', '3.5.', '4.5.', '5.5.'
];
/*
var dailyUsage = [];
for(let i=0; i < 16; i++) { dailyUsage.push(Math.random(0,30)) }

var cumUsage = [];
var cumCounter = 0
dailyUsage.map((used) => {
	cumUsage.push(cumCounter + used)
	cumCounter += used
})
*/

/*
const data = {
	labels: labels,
	datasets: [{
		borderWidth: 1,
		pointRadius: 2,
		label: 'Temperature',
		backgroundColor: 'rgb(255, 99, 132)',
		borderColor: 'rgb(255, 99, 132)',
		data: cumUsage,
		type: 'line',
		order: 0
	},{
		borderWidth: 1,
		pointRadius: 2,
		label: 'Pressure',
		data: dailyUsage,
		type: 'line',
		borderColor: 'rgb(0 140 220)',
		backgroundColor: 'rgb(55 162 235)',
		order: 1
	}]
};
*/

export function time_axis(from, to) {
	label_count = 12;
	console.log(from, to, label_count);
	labels = [];
	return labels;
	for(let i = 0; i < label_count; i++) {
		dailyUsage.push(Math.random(0,30))
	}
	return labels;
}

export default {
	props: {
		period: { required: true },
		config: { required: true }
	},

	data: () => ({
		// TODO: this could be calculated from the number of rows, no?
		height: 180
	}),

	computed: {
		//
	},

	methods: {
		// newData must be an array
		addData(newData, clear = false) {
			const data = this.chart.data;
			if (data.datasets.length > 0) {
				// data.labels = time_axis(...this.period.getPeriod());

				// push or replace
				if (clear) {
					data.datasets[0].data = newData;
				} else {
					newData.forEach(d => data.datasets[0].data.push(d));
				}

				console.log(data);

				this.chart.update();
			}
		}
	},

	mounted() {
		this.chart = new Chart(
			this.$refs.chartCanvas,
			this.config
		);
	},
}
</script>