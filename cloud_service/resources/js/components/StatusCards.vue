<template>
    <div class="row row-cols-1 row-cols-md-3 g-3">
        <div class="col">
            <div class="card">
                <div class="card-body">
                    <h5 class="card-title">Gateways</h5>
                    <h6 class="card-subtitle mb-2 text-muted">Disabled: {{ stats.disabled_gateways || 0 }}</h6>
                    <!--<p class="card-text">Some quick example text to build on the card title and make up the bulk of the card's content.</p>-->
                    <h1>{{ stats.total_gateways || 0 }}</h1>
                    <a href="#" class="card-link">
                        <i class="fas fa-sitemap"></i>
                        View gateways
                    </a>
                    <!--<a href="#" class="card-link">Another link</a>-->
                </div>
            </div>
        </div>
        <div class="col">
            <div class="card">
                <div class="card-body">
                    <h5 class="card-title">Alerts</h5>
                    <h6 class="card-subtitle mb-2 text-muted">Muted: {{ stats.muted_alerts || 0 }}</h6>
                    <!--<p class="card-text">Some quick example text to build on the card title and make up the bulk of the card's content.</p>-->
                    <h1>{{ stats.total_alerts || 0 }}</h1>
                    <a href="#" class="card-link">
                        <i class="far fa-bell"></i>
                        View alerts
                    </a>
                <!--<a href="#" class="card-link">Another link</a>-->
                </div>
            </div>
        </div>
        <div class="col">
            <div class="card">
                <div class="card-body">
                    <h5 class="card-title">Measurements</h5>
                    <h6 class="card-subtitle mb-2 text-muted">Latest {{ stats.latest_measurement || 0 }}</h6>
                    <!--<p class="card-text">Some quick example text to build on the card title and make up the bulk of the card's content.</p>-->
                    <h1>{{ stats.total_measurements || 0 }}</h1>
                    <a href="#" class="card-link">
                        <i class="far fa-list-alt"></i>
                        View measurements
                    </a>
                    <!--<a href="#" class="card-link">Another link</a>-->
                </div>
            </div>
        </div>
    </div>
</template>
<script>
export default {
    data: () => ({
        stats: {}
    }),
    mounted() {
        this.getStats();
    },
    computed: {
        //
    },
    methods: {
        getStats() {
            var mainContext = this;

            Api.get('stats')
            .then(response => {
                mainContext.stats = response.data;
            })
            .catch(error => {
                console.log(error)
            });
        }
    }
}
</script>