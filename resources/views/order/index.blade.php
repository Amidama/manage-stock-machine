@extends('layouts.app')

@section('content')
<div class="container">
    <div class="row justify-content-center">
        <div class="col-md-12">
            <div class="card">
                <div class="card-header">Orders</div>

                <div class="card-body">
                    @if (session('status'))
                        <div class="alert alert-success" role="alert">
                            {{ session('status') }}
                        </div>
                    @endif

                    <table id="myTable" class="table table-striped table-bordered" style="width:100%" >
                        <thead class="thead-dark">
                            <tr align="center">
                            <th scope="col">#</th>
                            <th scope="col">Tool Name</th>
                            <th scope="col">Amount</th>
                            <th scope="col">Status</th>
                            <th scope="col">Action</th>
                            </tr>
                        </thead>
                        <tbody>
                            @foreach ($orders as $index=>$order)
                            <tr align="center">
                            <td scope="row">{{ ++$index }}</td>
                            <td scope="row">{{$order->tool()->first()->name}}</td>
                            <td scope="row">{{$order->amount}}</td>
                            <td scope="row">{{$order->status == 'stock_in' ? 'นำเข้า' : 'นำออก'}}</td>
                            <td>
                                <form action="{{route('order.destroy')}}" method="post">
                                    @csrf
                                    <input id='id' name='id' value='{{$order->id}}' type='hidden'>
                                    <button type="submit" class="btn btn-danger" onclick="return confirm('Are you sure?')">
                                        Delete
                                    </button>
                                </form>
                            </td>
                            </tr>
                            @endforeach
                        </tbody>
                        <tfoot class="thead-dark">
                            <tr align="center">
                            <th scope="col">#</th>
                            <th scope="col">Tool Name</th>
                            <th scope="col">Amount</th>
                            <th scope="col">Status</th>
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
