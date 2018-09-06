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

namespace Zephir\Cache;

use Zephir\CompilationContext;

/**
 * ClassEntryCache
 *
 * Classes located in the PHP userland are cached to avoid further relocates
 */
class ClassEntryCache
{
    protected $cache = array();

    /**
     * Retrieves/Creates a class entry cache
     *
     * @param string $className
     * @param boolean $dynamic
     * @param CompilationContext $compilationContext
     */
    public function get($className, $dynamic, CompilationContext $compilationContext)
    {

        /**
         * Creates a guard variable if the class name is not dynamic
         */
        if (!$dynamic) {
            $zendClassEntry = $compilationContext->symbolTable->addTemp('static_zend_class_entry', $compilationContext);
            $zendClassEntry->setMustInitNull(true);

            $compilationContext->backend->fetchClass($zendClassEntry, $className, true, $compilationContext);
        } else {
            $zendClassEntry = $compilationContext->symbolTable->addTemp('zend_class_entry', $compilationContext);
            $compilationContext->backend->fetchClass($zendClassEntry, $className, false, $compilationContext);
        }

        return $zendClassEntry;
    }
}
