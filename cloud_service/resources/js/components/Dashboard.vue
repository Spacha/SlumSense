<template>
    <div>
        <!-- Status cards -->
        <status-cards ref="stats" :user-is-admin="userIsAdmin" />

        <!-- Manager panel -->
        <management-panel v-if="userIsAdmin" />

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
        userIsAdmin() {
            return this.user.is_admin == "true"
        }
    },
    methods: {
        refresh() {
            this.$refs.stats.getStats();
            this.$refs.measurements.getMeasurements();
        }
    }
}
</script>