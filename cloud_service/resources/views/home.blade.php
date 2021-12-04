@extends('layouts.app')

@section('content')
<div class="container">
    <div class="row justify-content-center">
        <div class="col-md-8">
            <div class="card">
                <div class="card-header">{{ __('Dashboard') }}</div>

                <div class="card-body">
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

            <div class="card mt-3">
                <div class="card-header">{{ __('Measurements') }}</div>
                <div class="card-body">
                    <!-- VUE -->
                    <div id="app">
                    <!-- VUE -->
                </div>
            </div>
        </div>
    </div>
</div>
@endsection
