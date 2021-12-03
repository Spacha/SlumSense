<?php

namespace App\Http\Requests;

use Illuminate\Http\Request;
use Illuminate\Foundation\Http\FormRequest;

use App\Models\Gateway;

class StoreMeasurementRequest extends FormRequest
{
    /**
     * Determine if the user is authorized to make this request.
     *
     * @return bool
     */
    public function authorize(Request $request)
    {
        // authorize gateway
        $gatewayKey = $request->input('gateway_key', '');
        $gateway = Gateway::byKey($gatewayKey)->first();

        if (!$gateway || $gateway->is_disabled) {
            return false;
        }

        return true; // OK
    }

    /**
     * Get the validation rules that apply to the request.
     *
     * @return array
     */
    public function rules()
    {
        return [
            'gateway_key'   => 'required|string|min:8|max:8|exists:gateways,key',
            'temperature'   => 'required|numeric|between:-70.0,70.0',
            'pressure'      => 'required|numeric|between:0.0,3000.0',
            'humidity'      => 'required|numeric|between:0.0,100.0',
        ];
    }
}
