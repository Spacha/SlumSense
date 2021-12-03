<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Http\Requests\StoreMeasurementRequest;
use App\Http\Requests\UpdateMeasurementRequest;
use App\Models\Measurement;
use App\Models\Gateway;

class ApiController extends Controller
{
    /**
     * Create a new controller instance.
     *
     * @return void
     */
    public function __construct()
    {
        //
    }

    /**
     * Store a new measurement in storage.
     *
     * @param  \App\Http\Requests\StoreMeasurementRequest  $request
     * @return \Illuminate\Http\Response
     */
    public function store(StoreMeasurementRequest $request)
    {
        $validated = $request->safe()->only([
            'gateway_key', 'temperature', 'pressure', 'humidity'
        ]);

        $gateway = Gateway::findByKey($validated['gateway_key']);

        // create a new measurement resource
        $measurement = new Measurement();
        $measurement->gateway_id    = $gateway->id;
        $measurement->temperature   = $validated['temperature'];
        $measurement->pressure      = $validated['pressure'];
        $measurement->humidity      = $validated['humidity'];
        $measurement->version       = env('API_VERSION');

        // try saving the measurement
        $success = $measurement->save();

        if (!$success) {
            return response()->json([
                "message" => "Error saving measurement"
            ], 400);
        }

        return response()->json([
            "message" => "Measurement stored",
        ], 201);
    }
}