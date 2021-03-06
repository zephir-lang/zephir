<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

/**
 * ClassDefinitionRuntime.
 *
 * Represents a class/interface that only exists in runtime
 */
class ClassDefinitionRuntime
{
    private $className;

    public function __construct($className)
    {
        $this->className = $className;
    }

    public function getName()
    {
        return $this->className;
    }
}
