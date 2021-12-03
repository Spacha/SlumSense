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

                    <table class="table">
                        <!--<thead>
                            <tr>
                                <th scope="col"></th>
                                <th scope="col"></th>
                            </tr>
                        </thead>-->
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
                </div>

            </div>
            <div class="card mt-3">

                <div id="app"></div>

            </div>
        </div>
    </div>
</div>
@endsection
