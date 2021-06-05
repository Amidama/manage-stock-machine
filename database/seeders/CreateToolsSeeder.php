<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use App\Models\Tool;

class CreateToolsSeeder extends Seeder
{
  /**
   * Run the database seeds.
   *
   * @return void
   */
  public function run()
  {
    $tools = [
      [
        'name' => 'nut',
        'type' => 1,
        'amount' => '0',
        'unit' => 'kilogram'
      ],
      [
        'name' => 'nail',
        'type' => 2,
        'amount' => '1.5',
        'unit' => 'kilogram'
      ],[
        'name' => 'wire',
        'type' => 0,
        'amount' => '5',
        'unit' => 'item'
      ],
    ];

    foreach ($tools as $key => $tool) {
      Tool::create($tool);
    }
  }
}
