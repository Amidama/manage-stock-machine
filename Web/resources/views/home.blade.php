@extends('layouts.app')

@section('content')
<div class="container">
    <div class="row justify-content-center">
        <div class="col-md-8">
            <div class="card">
                <div class="card-header">{{ __('Dashboard') }}</div>

                <div class="card-body" style="text-align: center;">
                    @if (session('status'))
                        <div class="alert alert-success" role="alert">
                            {{ session('status') }}
                        </div>
                    @endif

                    {{ __('Welcome!') }}

                    <br/>
                    <div class="content-center">
                      <a href="{{ route('tool.index') }}" type="button" class="btn btn-info margin-button">Remaining tool list</a>
                      <a href="{{ route('order.index') }}" type="button" class="btn btn-info margin-button">Logging</a>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
@endsection
