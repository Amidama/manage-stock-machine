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
                                <label for="book_name">Name</label>
                                <input type="text" class="form-control" id="card_id" placeholder="Card ID" name="card_id" value="{{ $tool->name }}" required>
                            </div>
                            <div class="form-group">
                                <label for="book_name">Amount</label>
                                <input type="text" class="form-control" id="student_id" placeholder="Student ID" name="student_id" value="{{ $tool->amount }}" required>
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
