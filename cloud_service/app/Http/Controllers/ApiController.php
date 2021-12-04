<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Events\MeasurementStored;
use App\Http\Requests\StoreMeasurementRequest;
use App\Http\Requests\UpdateMeasurementRequest;
use App\Models\Measurement;
use App\Models\Gateway;

use Carbon\Carbon;

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
     * Display a listing of the measurements.
     *
     * @return \Illuminate\Http\Response
     */
    public function listMeasurements(Request $request)
    {
        // if 'from' is not provided, set the beginning to year 1970...
        // if 'to' is not provided, set the end to current time...
        $from = Carbon::createFromTimestamp($request->input('from') ?? 0);
        $to = Carbon::now();

        if (!empty($request->input('to')))
            $to->timestamp = $request->input('to');

        $measurements = Measurement::whereBetween('created_at', [$from, $to])->get();

        return response()->json($measurements);
    }

    /**
     * Store a new measurement in storage.
     *
     * @param  \App\Http\Requests\StoreMeasurementRequest  $request
     * @return \Illuminate\Http\Response
     */
    public function storeMeasurement(StoreMeasurementRequest $request)
    {
        $validated = $request->safe()->only([
            'gateway_key', 'temperature', 'pressure', 'humidity'
        ]);

        $gateway = Gateway::byKey($validated['gateway_key'])->first();

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

        // dispatch an event for websockets
        // MeasurementStored::dispatch($measurement);

        return response()->json([
            "message" => "Measurement stored",
        ], 201);
    }
}
