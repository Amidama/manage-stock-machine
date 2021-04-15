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
        'amount' => '20',
        'unit' => 'item'
      ],
      [
        'name' => 'nail',
        'amount' => '40',
        'unit' => 'item'
      ],[
        'name' => 'wire',
        'amount' => '100',
        'unit' => 'meter'
      ],
    ];

    foreach ($tools as $key => $tool) {
      Tool::create($tool);
    }
  }
}
