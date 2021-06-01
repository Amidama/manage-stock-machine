@extends('layouts.app')

@section('content')
<div class="container">
    <div class="row justify-content-center">
        <div class="col-md-8">
            <div class="card">
                <div class="card-header">Edit</div>

                <div class="card-body">
                    @if (session('status'))
                        <div class="alert alert-success" role="alert">
                            {{ session('status') }}
                        </div>
                    @endif

                    <div class="container">

                    <div class="row">
                      <div class="col-md-12">
                        <h3>Form Edit Tool : {{ $tool->name }}</h3>
                      </div>
                    </div>

                    <div class="row">
                        <div class="col-md-12">
                        <div class="row">
                        </div>
                          <form action="{{route('tool.update')}}" method="post">
                            @csrf
                            <input id='id' name='id' value='{{$tool->id}}' type='hidden'>
                            <div class="form-group">
                                <label>Name</label>
                                <input type="text" class="form-control" id="name" placeholder="Text" name="name" value="{{ $tool->name }}" required>
                            </div>
                            <div class="form-group">
                                <label>Amount</label>
                                <input type="number" class="form-control" id="amount" placeholder="Number" name="amount" value="{{ $tool->amount }}" required>
                            </div>
                            <button type="submit" class="btn btn-primary">Submit</button>
                            <a href="{{route('tool.index')}}"><input type="button" class="btn btn-danger" value="Cancel"></a>
                          </form>
                        </div>
                    </div>
                </div>
   
                </div>
            </div>
        </div>
    </div>
    <br>
    <div class="row justify-content-center">
      <div class="col-md-8">
        <a class="btn btn-primary" href="{{route('tool.index')}}"><< Back</a>
      </div>
    </div>
</div>
@endsection
