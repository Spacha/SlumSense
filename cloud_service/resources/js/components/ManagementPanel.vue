<template>
    <div class="card mt-3">
        <div class="card-header d-flex align-items-center">
            Management
            <button class="btn btn-secondary btn-sm ms-auto" type="button" data-bs-toggle="collapse" data-bs-target="#collapsableManagement" aria-expanded="false" aria-controls="collapsableManagement">
                Show/hide
            </button>
        </div>

        <div class="card-body collapse" id="collapsableManagement">

            <h4>API general</h4>
            <table class="table table-bordered">
                <tbody>
                    <tr>
                        <th scope="row">API key</th>
                        <td><code>{{ management.api_key }}</code></td>
                    </tr>
                    <tr>
                        <th scope="row">API key (MD5)</th>
                        <td><code>{{ management.api_key_md5 }}</code></td>
                    </tr>
                </tbody>
            </table>

            <h4>Gateways</h4>
            <table class="table table-bordered">
                <thead>
                    <tr>
                        <th scope="col">#</th>
                        <th scope="col">Name</th>
                        <th scope="col">Key</th>
                        <th scope="col">State</th>
                    </tr>
                </thead>
                <tbody>
                    <tr v-for="gateway in gateways">
                        <th scope="row">{{ gateway.id }}</th>
                        <td>{{ gateway.name }}</td>
                        <td><code>{{ gateway.key }}</code></td>
                        <td class="fw-bold" :class="{'text-success': gateway.is_active, 'text-danger': !gateway.is_active}">
                                <span v-if="gateway.is_active" class="badge bg-success">Active</span>
                                <span v-else class="badge bg-danger">Disabled</span>
                        </td>
                    </tr>

                    <tr v-if="gateways.length == 0">
                        <td colspan="3">No gateways.</td>
                    </tr>
                </tbody>
            </table>                        
        </div>
    </div>
</template>
<script>

export default {
    data: () => ({
        gateways: [],
        management: {}
    }),
    mounted() {
        Api.get('management')
            .then(res => {
                this.management = res.data.management;
                this.gateways = res.data.gateways;
            })
            .catch(err => console.log(err))
    },
    computed: {
        //
    },
    methods: {
        //
    }
}
</script>
