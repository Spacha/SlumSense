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
        return !empty($this->disabled_at) && ($this->disabled_at < now());
    }

    /**
     * Get the first gateway that has the given key.
     *
     * @param \Illuminate\Database\Eloquent\Builder $query
     * @param string $key
     * @return \Illuminate\Database\Eloquent\Builder
     */
    public function scopeFindByKey($query, $key)
    {
        return $query->where('key', $key)->first();
    }
}
