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

namespace Zephir\Cache;

use Zephir\CompilationContext;
use Zephir\Variable;

/**
 * Classes located in the PHP userland are cached to avoid further relocates
 */
class ClassEntryCache
{
    protected array $cache = [];

    /**
     * Retrieves/Creates a class entry cache.
     */
    public function get(string $className, bool $dynamic, CompilationContext $compilationContext): Variable
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
