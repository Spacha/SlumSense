@extends('layouts.app')

@section('content')
<div class="container-lg">
    <div class="row justify-content-center">
        <div class="col-md-10 custom-col-container">

            <!-- Status cards -->

            <div class="row row-cols-1 row-cols-md-3 g-3">
                <div class="col">
                    <div class="card">
                        <div class="card-body">
                            <h5 class="card-title">Gateways</h5>
                            <h6 class="card-subtitle mb-2 text-muted">Disabled: {{ $stats->disabledGateways }}</h6>
                            <!--<p class="card-text">Some quick example text to build on the card title and make up the bulk of the card's content.</p>-->
                            <h1>{{ $stats->totalGateways }}</h1>
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
                            <h6 class="card-subtitle mb-2 text-muted">Muted: {{ $stats->mutedAlerts }}</h6>
                            <!--<p class="card-text">Some quick example text to build on the card title and make up the bulk of the card's content.</p>-->
                            <h1>{{ $stats->totalAlerts }}</h1>
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
                            <h6 class="card-subtitle mb-2 text-muted">Total: {{ $stats->totalMeasurements }}</h6>
                            <!--<p class="card-text">Some quick example text to build on the card title and make up the bulk of the card's content.</p>-->
                            <h1>{{ $stats->totalMeasurements }}</h1>
                            <a href="#" class="card-link">
                                <i class="far fa-list-alt"></i>
                                View measurements
                            </a>
                            <!--<a href="#" class="card-link">Another link</a>-->
                        </div>
                    </div>
                </div>
            </div>

            <!-- Manager panel -->

            @if (Auth::user()->isAdmin())
            <div class="card mt-3">
                <div class="card-header d-flex align-items-center">
                    {{ __('Management') }}
                    <button class="btn btn-secondary btn-sm ms-auto" type="button" data-bs-toggle="collapse" data-bs-target="#collapsableManagement" aria-expanded="false" aria-controls="collapsableManagement">
                        Show/hide
                    </button>
                </div>

                <div class="card-body collapse" id="collapsableManagement">
                    @if (session('status'))
                        <div class="alert alert-success" role="alert">
                            {{ session('status') }}
                        </div>
                    @endif

                    <h4>API general</h4>
                    <table class="table table-bordered">
                        <tbody>
                            <tr>
                                <th scope="row">API key</th>
                                <td><code>{{ env('API_KEY') }}</code></td>
                            </tr>
                            <tr>
                                <th scope="row">API key (MD5)</th>
                                <td><code>{{ md5(env('API_KEY')) }}</code></td>
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
                            @forelse($gateways as $gateway)
                            <tr>
                                <th scope="row">{{ $gateway->id }}</th>
                                <td>{{ $gateway->name }}</td>
                                <td><code>{{ $gateway->key }}</code></td>
                                <td class="fw-bold {{ $gateway->is_active ? 'text-success' : 'text-danger' }}">
                                    @if($gateway->is_active)
                                        <span class="badge bg-success">Active</span>
                                    @else
                                        <span class="badge bg-danger">Disabled</span>
                                    @endif
                                </td>
                            </tr>
                            @empty
                            <tr>
                                <td colspan="3">No gateways.</td>
                            </tr>
                            @endforelse
                        </tbody>
                    </table>                        
                </div>
            </div>
            @endif

            <div class="card mt-3">
                <div class="card-header">{{ __('Measurements') }}</div>

                    <!-- VUE -->
                    <div id="app"></div>
                    <!-- VUE -->

            </div>
        </div>
    </div>
</div>
@endsection
