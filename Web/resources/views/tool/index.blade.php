@extends('layouts.app')

@section('content')
<div class="container">
    <div class="row justify-content-center">
        <div class="col-md-12">
            <div class="card">
                <div class="card-header">
                    <div class="row">
                        <div class="col-md-9">
                            Tools
                        </div>
                        <div class="col-md-3" align="right">
                            <a href="{{route('tool.create')}}">
                                <button type="button" class="btn btn-outline-primary">+ Add Tool</button>
                            </a>
                        </div>
                    </div>
                </div>

                <div class="card-body">
                    @if (session('status'))
                        <div class="alert alert-success" role="alert">
                            {{ session('status') }}
                        </div>
                    @endif
                    
                    
                    <table class="table" id="myTable">
                        <thead class="thead-dark">
                            <tr align="center">
                            <th scope="col">#</th>
                            <th scope="col">Name</th>
                            <th scope="col">Type</th>
                            <th scope="col">Amount</th>
                            <th scope="col">Unit</th>
                            <th scope="col">Action</th>
                            </tr>
                        </thead>
                        <tbody>
                            @foreach ($tools as $index=>$tool)
                            <tr align="center">
                            <th scope="row">{{ ++$index }}</th>
                            <td>{{$tool->name}}</td>
                            <td>{{$tool->type}}</td>
                            <td>{{$tool->amount}}</td>
                            <td>{{$tool->unit}}</td>
                            <td>
                                <form action="{{route('tool.destroy')}}" method="post">
                                    @csrf
                                    <a href="{{route('tool.edit',$tool->id)}}">
                                        <input type="button" class="btn btn-warning" value='Edit'>
                                    </a>
                                    <input id='id' name='id' value='{{$tool->id}}' type='hidden'>
                                </form>
                            </td>
                            </tr>
                            @endforeach
                        </tbody>
                        <tfoot class="thead-dark">
                            <tr align="center">
                            <th scope="col">#</th>
                            <th scope="col">Name</th>
                            <th scope="col">Type</th>
                            <th scope="col">Amount</th>
                            <th scope="col">Unit</th>
                            <th scope="col">Action</th>
                            </tr>
                        </tfoot>
                    </table>
                </div>
            </div>
        </div>
    </div>
    <br>
    <a class="btn btn-primary" href="{{route('home')}}"><< Back</a>
</div>
@endsection
