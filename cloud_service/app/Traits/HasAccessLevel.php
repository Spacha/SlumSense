<?php

namespace App\Traits;

use App\AccessLevel;

/**
 * ...
 */
trait HasAccessLevel
{
    /**
     * Determine if the user is an administrator.
     *
     * @return bool
     */
    public function getIsAdminAttribute()
    {
        return $this->isAdmin();
    }

    /******************************************************
     * General methods
     *****************************************************/

    public function isViewer() : bool
    {
        return $this->access_level == AccessLevel::ACCESS_LEVEL_VIEWER;
    }

    public function isManager() : bool
    {
        return $this->access_level == AccessLevel::ACCESS_LEVEL_ADMIN;
    }

    public function isAdmin() : bool
    {
        return $this->access_level == AccessLevel::ACCESS_LEVEL_ADMIN;
    }

    public function isAtLeastViewer() : bool
    {
        return $this->access_level >= AccessLevel::ACCESS_LEVEL_VIEWER;
    }

    public function isAtLeastManager() : bool
    {
        return $this->access_level >= AccessLevel::ACCESS_LEVEL_MANAGER;
    }

    public function isAtLeastAdmin() : bool
    {
        return $this->access_level >= AccessLevel::ACCESS_LEVEL_ADMIN;
    }

    public static function getRole() : string
    {
        return AccessLevel::getAccessLevelRole($this->access_level);
    }
}
