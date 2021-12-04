<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Models\Gateway;
use App\Models\Measurement;

class HomeController extends Controller
{
    /**
     * Create a new controller instance.
     *
     * @return void
     */
    public function __construct()
    {
        $this->middleware('auth');
    }

    /**
     * Show the application dashboard.
     *
     * @return \Illuminate\Contracts\Support\Renderable
     */
    public function index()
    {
        $stats = new \stdClass();
        $stats->disabledGateways    = Gateway::disabled()->count();
        $stats->totalGateways       = Gateway::count();
        $stats->totalMeasurements   = Measurement::count();
        $stats->totalAlerts         = 0;
        $stats->mutedAlerts         = 0;

        return view('home', [
            'gateways' => Gateway::all(),
            'measurements' => Measurement::all(),
            'stats' => $stats
        ]);
    }
}
