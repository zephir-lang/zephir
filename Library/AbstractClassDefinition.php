<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir;

abstract class AbstractClassDefinition
{
    /**
     * Class name
     *
     * @var string
     */
    protected string $name;

    /**
     * Returns the class name.
     *
     * @return string
     */
    public function getName(): string
    {
        return $this->name;
    }
}
