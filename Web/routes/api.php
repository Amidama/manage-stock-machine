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

Route::middleware('auth:api')->get('/user', function (Request $request) {
    return $request->user();
});

Route::post('/hardware/stockin', [App\Http\Controllers\HardwareController::class, 'stockin'])->name('hardware.stockin');
Route::post('/hardware/stockout', [App\Http\Controllers\HardwareController::class, 'stockout'])->name('hardware.stockout');