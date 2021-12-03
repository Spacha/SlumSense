<?php

namespace App\Http\Middleware;

use Closure;
use Illuminate\Http\Request;
use App\Models\Gateway;

class AuthenticateApi
{
    /**
     * Handle an incoming request.
     *
     * @param  \Illuminate\Http\Request  $request
     * @param  \Closure  $next
     * @return mixed
     */
    public function handle(Request $request, Closure $next)
    {
        $apiKey = $request->input('API_KEY') ?? '';

        if ($apiKey != md5( env('API_KEY') ))
            return $this->handleUnauthorized("Invalid API key.");

        // authorize gateway
        $gatewayKey = $request->input('gateway_key', '');
        $gateway = Gateway::findByKey($gatewayKey);

        if (empty($gateway) || $gateway->is_disabled)
            return $this->handleUnauthorized("Unauthorized gateway.");

        return $next($request);
    }

    /**
     * 
     */
    private function handleUnauthorized(string $message)
    {
        return response()->json(["message" => $message], 401);
    }
}
