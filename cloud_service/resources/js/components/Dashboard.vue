<template>
    <div>
        <!-- Status cards -->
        <status-cards ref="stats" />

        <!-- Manager panel -->
        <management-panel v-if="user.is_admin" />

        <div class="card mt-3">
            <div class="card-header">Measurements</div>
            <measurements ref="measurements" />
        </div>
    </div>
</template>
<script>
const MAX_MEASUREMENTS = 50;

export default {
    data: () => ({
        user: {}
    }),
    mounted() {
        setInterval(this.refresh, 10e3);

        Api.get('user')
            .then(res => this.user = res.data)
            .catch(err => console.log(err));
    },
    computed: {
        //
    },
    methods: {
        refresh() {
            this.$refs.stats.getStats()
            this.$refs.measurements.getMeasurements()
        }
    }
}
</script>