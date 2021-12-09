<?php

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;

/*
|--------------------------------------------------------------------------
| API Routes
|--------------------------------------------------------------------------
|
| Here is where you can register API routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| is assigned the "api" middleware group. Enjoy building your API!
|
*/

Route::prefix('v1')->group(function() {
    // Cloud API
    Route::get('measurements', [App\Http\Controllers\ApiController::class, 'listMeasurements']);
    Route::get('stats', [App\Http\Controllers\ApiController::class, 'getStats']);

    // Gateway API
    Route::post('measurements', [App\Http\Controllers\ApiController::class, 'storeMeasurement']);
});