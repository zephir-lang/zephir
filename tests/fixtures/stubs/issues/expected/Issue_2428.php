<?php

namespace Stubs;

use Stubs\Events\ManagerInterface;
use Stubs\Events\EventManager;

/**
 * Union return types with more than one class must keep every class in the
 * generated signature.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2428
 */
class Issue_2428
{


    /**
     * @return ManagerInterface|EventManager|null
     */
    public function findFirst(): ManagerInterface|EventManager|null
    {
    }

    /**
     * @return bool|ManagerInterface|EventManager
     */
    public function eitherOrBool(): ManagerInterface|EventManager|bool
    {
    }
}
