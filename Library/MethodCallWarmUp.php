<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir;

/**
 * MethodCallWarmUp
 *
 * List of methods that require warm up
 */
class MethodCallWarmUp
{
    /**
     *
     */
    private $warmUps = array();

    /**
     *
     *
     * @param ClassDefinition $classDefinition
     * @param ClassMethod $method
     */
    public function add(ClassDefinition $classDefinition, ClassMethod $method)
    {
        $this->warmUps[] = array(
            $classDefinition,
            $method
        );
    }

    public function get()
    {
    }
}
