<?php

namespace Database\Seeders;

use App\Models\Order;
use App\Models\Tool;
use App\Models\User;
use Illuminate\Database\Seeder;
use Database\Seeders\CreateToolsSeeder;
use Database\Seeders\CreateUsersSeeder;
use Illuminate\Support\Carbon;
class DatabaseSeeder extends Seeder
{
    /**
     * Seed the application's database.
     *
     * @return void
     */
    public function run()
    {
      $this->call([
        CreateToolsSeeder::class,
        CreateUsersSeeder::class
    ]);

    Order::create(
      [
        'user_id' => User::all()->first()->id,
        'tool_id' => Tool::all()->first()->id,
        'amount' => 10,
        'started_at' => Carbon::now('Asia/Phnom_Penh'),
      ]
    );
    }
}
