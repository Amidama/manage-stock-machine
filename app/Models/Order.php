<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Order extends Model
{
    use HasFactory;

    protected $fillable = [
      'tool_id', 'amount', 'status'
    ];

    public function tool()
    {
      return $this->belongsTo('App\Models\Tool');
    }
  
    public function user()
    {
      return $this->belongsTo('App\Models\User');
    }
}
