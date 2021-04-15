<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateOrdersTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
      Schema::create('orders', function (Blueprint $table) {
          $table->increments('id', true);
          $table->integer('user_id')->unsigned();
          $table->integer('tool_id')->unsigned();
          $table->integer('amount');
          $table->dateTime('started_at');
          $table->dateTime('ended_at')->nullable();
          $table->timestamps();
      });

      Schema::table('orders', function($table) {
        $table->foreign('user_id')->references('id')->on('users')->onDelete('cascade');;
        $table->foreign('tool_id')->references('id')->on('tools')->onDelete('cascade');;
    });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('orders');
    }
}
