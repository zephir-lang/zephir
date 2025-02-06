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

namespace Zephir\Compiler;

use Zephir\Class\Definition\Definition;
use Zephir\Compiler;

/**
 * Provides a common interface for compiler files
 */
interface FileInterface
{
    /**
     * Returns the class definition related to the compiled file.
     *
     * @return Definition
     */
    public function getClassDefinition();

    /**
     * Checks if the class file belongs to an external dependency or not.
     *
     * @return bool
     */
    public function isExternal();

    /**
     * Pre-compiles a Zephir file.
     *
     * Generates the IR and perform basic validations.
     *
     * @param Compiler $compiler
     */
    public function preCompile(Compiler $compiler);

    /**
     * Sets if the class belongs to an external dependency or not.
     *
     * @param bool $external
     */
    public function setIsExternal($external);
}
