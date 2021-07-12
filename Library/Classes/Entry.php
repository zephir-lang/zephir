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

namespace Zephir\Classes;

use Zephir\ClassDefinition;
use Zephir\CompilationContext;
use Zephir\Exception;

/**
 * Zend Class Entry detector
 */
class Entry
{
    public const NAMESPACE_SEPARATOR = '\\';

    /**
     * Class name
     *
     * As it was passed: partially or fully.
     *
     * @var string
     */
    private string $classname;

    /**
     * Class namespace
     *
     * From where it is called.
     *
     * @var string
     */
    private string $classNamespace;

    /**
     * @var CompilationContext
     */
    private CompilationContext $compilationContext;

    /**
     * Entry constructor.
     *
     * @param string $className
     * @param string $classNamespace
     * @param CompilationContext $compilationContext
     */
    public function __construct(string $className, string $classNamespace, CompilationContext $compilationContext)
    {
        $this->classname = $className;
        $this->classNamespace = $classNamespace;
        $this->compilationContext = $compilationContext;
    }

    /**
     * @return string
     * @throws Exception
     */
    public function get(): string
    {
        $className = $this->compilationContext->getFullName($this->classname);
        $classNamespace = explode(self::NAMESPACE_SEPARATOR, $className);

        /**
         * External class, we don't know its ClassEntry in C world.
         */
        if (!$this->isInternalClass($classNamespace[0])) {
            $className = str_replace(self::NAMESPACE_SEPARATOR, self::NAMESPACE_SEPARATOR.self::NAMESPACE_SEPARATOR, $className);

            return sprintf(
                'zend_lookup_class_ex(zend_string_init_fast(SL("%s%s%s")), NULL, 0)',
                self::NAMESPACE_SEPARATOR,
                self::NAMESPACE_SEPARATOR,
                $className
            );
        }

        $className = end($classNamespace);
        array_pop($classNamespace);
        $namespace = join(self::NAMESPACE_SEPARATOR, $classNamespace);

        return (new ClassDefinition($namespace, $className))->getClassEntry();
    }

    /**
     * Detect if start of namespace class
     * belongs to project namespace.
     *
     * @param string $className
     * @return bool
     */
    public function isInternalClass(string $className): bool
    {
        return preg_match('/^'.$className.'/', $this->classNamespace) === 1;
    }
}
