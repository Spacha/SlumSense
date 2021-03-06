<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Carbon\Carbon;
use Auth;

use App\Models\Gateway;
use App\Models\Measurement;

class DashboardController extends Controller
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
    public function home()
    {
        return view('home');
    }

    /**
     * ...
     *
     * @return \Illuminate\Http\Response
     */
    public function management()
    {
        if (!Auth::user()->isAdmin())
            return response()->json(["message" => "Unauthorized"], 401);

        return response()->json([
            'gateways'          => Gateway::all(),
            'management' => [
                'api_key'       => env('API_KEY'),
                'api_key_md5'   => md5(env('API_KEY'))
            ]
        ]);
    }

    /**
     * ...
     *
     * @return \Illuminate\Http\Response
     */
    public function user()
    {
        return response()->json(
            Auth::user()
        );
    }

    /**
     * Get general statistics for the dashboard.
     *
     * @return \Illuminate\Http\Response
     */
    public function statistics()
    {
        // Get some statistics
        $latestMeasurement = Measurement::latest()->first();

        $stats = new \stdClass();
        $stats->disabled_gateways    = Gateway::disabled()->count();
        $stats->total_gateways       = Gateway::count();
        $stats->latest_measurement   = !empty($latestMeasurement) ? $latestMeasurement->created_at->diffForHumans() : '';
        $stats->total_measurements   = Measurement::count();
        $stats->total_alerts         = 0;
        $stats->muted_alerts         = 0;

        return response()->json($stats);
    }

    /**
     * Get a listing of the measurements.
     *
     * @return \Illuminate\Http\Response
     */
    public function measurements(Request $request)
    {
        /**
         * Filters:
         *     from:    Unix timestamp, self explanatory
         *     to:      Unix timestamp, self explanatory
         *     softmax: If defined, evenly skip some results to decrease
         *              result size. Basically a downsampler, but worse
         *              (see the comment below).
         */

        // if 'from' is not provided, set the beginning to year 1970...
        // if 'to' is not provided, set the end to current time...
        $from = Carbon::createFromTimestamp($request->input('from') ?? 0);
        $to = Carbon::now();
        if (!empty($request->input('to')))
            $to->timestamp = $request->input('to');

        $softmax = $request->input('softmax') ?? 0;

        $measurements = Measurement::whereBetween('created_at', [$from, $to]);

        // downsamling logic...
        if ($softmax > 0) {
            $totalCount = $measurements->count();
            $downsampling = floor($totalCount / $softmax);

            /**
             * Softmax:
             *     Using softmax, this method may return at worst 2*softmax-1
             *     results, but it is quaranteed to return at least softmax
             *     results if that manu results are found matching other
             *     filters.
             *     So, if a 'hardmax' is wanted, one could set
             *     max = 2*softmax-1, but then tehy might get at worst
             *     max/2-1 less results than expected.
             *
             *     Also, some results may be added due to the 'recency inclusion',
             *     which includes all the recent results (within other filters).
             */

            // take every ($downsampling)th row,
            // but always include most recent data
            if ($downsampling > 1)
                $measurements->whereRaw("id MOD {$downsampling} = 0")
                    ->orWhere('created_at', '>=', $to->subMinute(5));
            /**
             * Better downsampling idea:
             *     Need to reduce results from $totalCount to $max.
             *     We will do it in 2 parts. First, get every Nth row, then
             *     fill in the rest by getting L latest rows.
             *     For example, when N = 2 and max = 9:
             *         every Nth:   [0,2,4,6,8,10,12,14]
             *         this leaves us with only 8 < max values
             *         fill to max: [0,2,4,6,8,10,12,(13),14]
             *     For example, when N = 3 and max = 15:
             *         every Nth:   [0,3,6,9,12,15,18,21,24,27,30,33,36]
             *         this leaves us with only 13 < max values
             *         fill to max: [0,3,6,9,12,15,18,21,24,27,30,(32),33,(34),36]
             */
        }

        return response()->json(
            $measurements->orderByDesc('created_at')->get()
        );
    }
}
