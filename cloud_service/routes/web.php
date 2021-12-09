<?php

use Illuminate\Support\Facades\Route;

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', function () {
    return view('welcome');
});

Auth::routes(['register' => env('APP_ALLOW_NEW_USERS')]);

// Cloud Dashboard
Route::get('/home', [App\Http\Controllers\DashboardController::class, 'home'])->name('home');
Route::get('/user', [App\Http\Controllers\DashboardController::class, 'user']);
Route::get('/stats', [App\Http\Controllers\DashboardController::class, 'statistics']);
Route::get('/management', [App\Http\Controllers\DashboardController::class, 'management']);
Route::get('/measurements', [App\Http\Controllers\DashboardController::class, 'measurements']);

Route::get('/measurements/{measurement}', [App\Http\Controllers\MeasurementController::class, 'show']);
