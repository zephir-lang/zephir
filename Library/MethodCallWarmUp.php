<?php

/**
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
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
