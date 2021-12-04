/**
 * First we will load all of this project's JavaScript dependencies which
 * includes Vue and other libraries. It is a great starting point when
 * building robust, powerful web applications using Vue and Laravel.
 */

import Vue from 'vue';
require('./bootstrap');

// Moment.js for time and date
window.moment = require('moment');

// Chart.js for graphing
// import { Chart } from 'chart.js';
// import 'chartjs-adapter-moment';

// Local components
import { api_url, Format } from './helpers.js';

window.api_url = api_url;
window.format = Format;             // for access outside Vue
Vue.prototype.$format = Format;     // for access within Vue

// console.log(window.Format.temperature(12.333))

/**
 * The following block of code may be used to automatically register your
 * Vue components. It will recursively scan this directory for the Vue
 * components and automatically register them with their "basename".
 *
 * Eg. ./components/ExampleComponent.vue -> <example-component></example-component>
 */

// const files = require.context('./', true, /\.vue$/i)
// files.keys().map(key => Vue.component(key.split('/').pop().split('.')[0], files(key).default))

Vue.component('example-component', require('./components/ExampleComponent.vue').default);
Vue.component('line-chart', require('./components/LineChart.vue').default);

/**
 * Next, we will create a fresh Vue application instance and attach it to
 * the page. Then, you may begin adding components to this application
 * or customize the JavaScript scaffolding to fit your unique needs.
 */

const app = new Vue({
    el: '#app',
    render: h => h('example-component')
});
