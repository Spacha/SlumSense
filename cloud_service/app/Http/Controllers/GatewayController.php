<?php

namespace App\Http\Controllers;

use App\Http\Requests\StoreGatewayRequest;
use App\Http\Requests\UpdateGatewayRequest;
use App\Models\Gateway;

class GatewayController extends Controller
{
    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index()
    {
        //
    }

    /**
     * Show the form for creating a new resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function create()
    {
        //
    }

    /**
     * Store a newly created resource in storage.
     *
     * @param  \App\Http\Requests\StoreGatewayRequest  $request
     * @return \Illuminate\Http\Response
     */
    public function store(StoreGatewayRequest $request)
    {
        //
    }

    /**
     * Display the specified resource.
     *
     * @param  \App\Models\Gateway  $gateway
     * @return \Illuminate\Http\Response
     */
    public function show(Gateway $gateway)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     *
     * @param  \App\Models\Gateway  $gateway
     * @return \Illuminate\Http\Response
     */
    public function edit(Gateway $gateway)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     *
     * @param  \App\Http\Requests\UpdateGatewayRequest  $request
     * @param  \App\Models\Gateway  $gateway
     * @return \Illuminate\Http\Response
     */
    public function update(UpdateGatewayRequest $request, Gateway $gateway)
    {
        //
    }

    /**
     * Remove the specified resource from storage.
     *
     * @param  \App\Models\Gateway  $gateway
     * @return \Illuminate\Http\Response
     */
    public function destroy(Gateway $gateway)
    {
        //
    }
}
