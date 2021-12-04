<?php

namespace App;

class AccessLevel
{
    /******************************************************
     * Access level definitions of the application
     *****************************************************/
    const ACCESS_LEVEL_VIEWER   = 0;
    const ACCESS_LEVEL_MANAGER  = 2;
    const ACCESS_LEVEL_ADMIN    = 5;

    const ROLES = [
        ACCESS_LEVEL_VIEWER     => 'viewer',
        ACCESS_LEVEL_MANAGER    => 'manager',
        ACCESS_LEVEL_ADMIN      => 'admin'
    ];
    /*****************************************************/

    public static function default() : int
    {
        return self::ACCESS_LEVEL_VIEWER;
    }

    public static function getRoleAccessLevel(string $role) : int
    {
        return array_search($role, self::ROLES) ?? 0;
    }

    public static function getAccessLevelRole(int $accessLevel) : string
    {
        return self::ROLES[$accessLevel] ?? 'unknown';
    }
}