<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

namespace Zephir;

/**
 * MethodCache
 *
 * Adds a local zend_function pointer to avoid relocate functions in every call
 */
class MethodCache
{
    private $_cache = array();

    /**
     * @param $functionName
     * @param CompilationContext $compilationContext
     * @return Variable
     */
    public function get($functionName, CompilationContext $compilationContext)
    {
        if (isset($this->_cache[$functionName])) {
            return $this->_cache[$functionName];
        }

        $functionCache = $compilationContext->symbolTable->getTempVariableForWrite('zend_function', $compilationContext);
        $functionCache->setMustInitNull(true);
        $functionCache->setReusable(false);
        $this->_cache[$functionName] = $functionCache;

        return $functionCache;
    }
}
