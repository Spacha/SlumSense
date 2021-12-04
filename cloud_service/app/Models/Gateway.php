<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Gateway extends Model
{
    use HasFactory;

    /**
     * Get if the gateway is currently active.
     *
     * @return string
     */
    public function getIsActiveAttribute()
    {
        return !$this->is_disabled;
    }

    /**
     * Get if the gateway is currently disabled.
     *
     * @return string
     */
    public function getIsDisabledAttribute()
    {
        return !empty($this->disabled_at) && ($this->disabled_at <= now());
    }

    /**
     * Get gateways with given key.
     *
     * @param \Illuminate\Database\Eloquent\Builder $query
     * @param string $key
     * @return \Illuminate\Database\Eloquent\Builder
     */
    public function scopeByKey($query, $key)
    {
        return $query->where('key', $key);
    }

    /**
     * Get gateways that are disabled.
     *
     * @param \Illuminate\Database\Eloquent\Builder $query
     * @return \Illuminate\Database\Eloquent\Builder
     */
    public function scopeDisabled($query)
    {
        return $query->whereNotNull('disabled_at')
            ->where('disabled_at', '<=', now());
    }
}
