<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

/**
 * MethodCallWarmUp.
 *
 * List of methods that require warm up
 */
class MethodCallWarmUp
{
    private $warmUps = [];

    /**
     * @param ClassDefinition $classDefinition
     * @param ClassMethod     $method
     */
    public function add(ClassDefinition $classDefinition, ClassMethod $method)
    {
        $this->warmUps[] = [
            $classDefinition,
            $method,
        ];
    }

    public function get()
    {
    }
}
