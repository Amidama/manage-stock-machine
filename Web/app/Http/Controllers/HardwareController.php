<?php

namespace App\Http\Controllers;

use App\Models\Order;
use App\Models\Tool;
use Illuminate\Http\Request;
use Exception;

class HardwareController extends Controller
{
  public function stockout(Request $request)
  {
    error_log('stockout');
    try {
      $tool = Tool::where('type', (int)$request->Type)->first();

      if ($tool == null) { throw new Exception('Type is invalid!'); };
      if ($tool->amount < (float)$request->Quantity) { throw new Exception('Not enough tools!'); };

      $tool->decrement('amount', $request->Quantity);

      Order::create(
        [
          'tool_id' => $tool->id,
          'amount' => (float)$request->Quantity,
          'status' => 'stock_out',
        ]
      );
      
      return response()->json([
          "status" => true,
          "message" => 'ok'
      ]);
    } catch (Exception $e) {
      return response()->json([
        "status" => false,
        "message" => $e->getMessage()
      ]);
    }
  }

  public function stockin(Request $request)
  {
    error_log('stockin');
    try {
      $tool = Tool::where('type', (int)$request->Type)->first();

      if ($tool == null) { throw new Exception('Type is invalid!'); };

      $tool->increment('amount', (float)$request->Quantity);

      Order::create(
        [
          'tool_id' => $tool->id,
          'amount' => (float)$request->Quantity,
          'status' => 'stock_in',
        ]
      );
      
      return response()->json([
          "status" => true,
          "message" => 'ok'
      ]);
    } catch (Exception $e) {
      return response()->json([
        "status" => false,
        "message" => $e->getMessage()
      ]);
    }
  }
}
