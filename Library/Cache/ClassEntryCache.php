<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Cache;

use Zephir\Call;
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

            $compilationContext->codePrinter->output('if (!' . $zendClassEntry->getName() . ') {');
            $compilationContext->codePrinter->output("\t" . $zendClassEntry->getName() . ' = zend_fetch_class(' . $className . ', ZEND_FETCH_CLASS_AUTO TSRMLS_CC);');
            $compilationContext->codePrinter->output('}');
        } else {
            $zendClassEntry = $compilationContext->symbolTable->addTemp('zend_class_entry', $compilationContext);
            $compilationContext->codePrinter->output($zendClassEntry->getName() . ' = zend_fetch_class(' . $className . ', ZEND_FETCH_CLASS_AUTO TSRMLS_CC);');
        }

        return $zendClassEntry;
    }
}
