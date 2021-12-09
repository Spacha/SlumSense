<template>
    <div>
        <!-- Status cards -->
        <status-cards ref="stats" />

        <!-- Manager panel -->
        <!-- <management-panel /> -->

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
        //
    }),
    mounted() {
        setInterval(this.refresh, 10e3);
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