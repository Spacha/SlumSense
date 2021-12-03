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
