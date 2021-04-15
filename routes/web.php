<?php

use Illuminate\Support\Facades\Route;

use App\Http\Controllers\HomeController;
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

Auth::routes();

Route::get('admin/home', [HomeController::class, 'adminHome'])->name('admin.home')->middleware('is_admin');
Route::get('/home', [App\Http\Controllers\HomeController::class, 'index'])->name('home');
Route::get('/tools', [App\Http\Controllers\ToolController::class, 'index'])->name('tool.index');
Route::get('/tool/edit/{id}', [App\Http\Controllers\ToolController::class, 'edit'])->name('tool.edit');
Route::get('/orders', [App\Http\Controllers\OrderController::class, 'index'])->name('order.index');
Route::post('/order/destroy', 'OrderController@destroy')->name('order.destroy');

Route::post('/tool/create', [App\Http\Controllers\ToolController::class, 'create'])->name('tool.create');
Route::post('/tool/update', [App\Http\Controllers\ToolController::class, 'update'])->name('tool.update');
Route::post('/tool/destroy', [App\Http\Controllers\ToolController::class, 'destroy'])->name('tool.destroy');

