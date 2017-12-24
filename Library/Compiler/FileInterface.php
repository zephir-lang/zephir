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

namespace Zephir\Compiler;

use Zephir\ClassDefinition;

/**
 * FileInterface
 *
 * Provides a common interface for compiler files
 */
interface FileInterface
{
    /**
     * Checks if the class file belongs to an external dependency or not.
     *
     * @return bool
     */
    public function isExternal();

    /**
     * Returns the class definition related to the compiled file.
     *
     * @return ClassDefinition
     */
    public function getClassDefinition();
}
