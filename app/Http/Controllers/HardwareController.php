<?php

namespace App\Http\Controllers;

use App\Models\Order;
use App\Models\Tool;
use Illuminate\Http\Request;
use Exception;

class HardwareController extends Controller
{
  public function fill(Request $request)
  {
    try {
      $tool = Tool::where('id', $request->tool_id)->first();

      if ($tool == null) { throw new Exception('Tool Id is invalid!'); };
      if ($tool->amount < $request->amount) { throw new Exception('Not enough tools!'); };

      $tool->decrement('amount', $request->amount);

      Order::create(
        [
          'tool_id' => $tool->id,
          'amount' => $request->amount,
          'status' => 'rent',
        ]
      );
      
      return response()->json([
          "status" => true,
          "message" => 'Successfully rent tools!'
      ]);
    } catch (Exception $e) {
      return response()->json([
        "status" => false,
        "message" => $e->getMessage()
      ]);
    }
  }

  public function return(Request $request)
  {
    try {
      $tool = Tool::where('id', $request->tool_id)->first();

      if ($tool == null) { throw new Exception('Tool Id is invalid!'); };

      $tool->increment('amount', $request->amount);

      Order::create(
        [
          'tool_id' => $tool->id,
          'amount' => $request->amount,
          'status' => 'return',
        ]
      );
      
      return response()->json([
          "status" => true,
          "message" => 'Successfully return tools!'
      ]);
    } catch (Exception $e) {
      return response()->json([
        "status" => false,
        "message" => $e->getMessage()
      ]);
    }
  }
}
