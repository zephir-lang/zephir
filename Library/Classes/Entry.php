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

use ReflectionClass;
use ReflectionException;
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
     * @var CompilationContext
     */
    private CompilationContext $compilationContext;

    /**
     * @var bool
     */
    private bool $isInternal = false;

    private array $undetectableClassEntries = [
        'ArrayObject' => 'spl_ce_ArrayObject',
    ];

    /**
     * Entry constructor.
     *
     * @param string $className
     * @param CompilationContext $compilationContext
     */
    public function __construct(string $className, CompilationContext $compilationContext)
    {
        $this->compilationContext = $compilationContext;
        $this->classname = $this->compilationContext->getFullName($className);
    }

    /**
     * @return string
     * @throws Exception
     * @throws ReflectionException
     */
    public function get(): string
    {
        if (isset($this->undetectableClassEntries[$this->classname])) {
            $this->compilationContext->headersManager->add('ext/spl/spl_array');

            return $this->undetectableClassEntries[$this->classname];
        }

        if (class_exists($this->classname)) {
            $reflection = new ReflectionClass($this->classname);
            $className = $reflection->getName();

            /**
             * Check if class is defined internally by an extension, or the core.
             */
            if ($reflection->isInternal()) {
                return sprintf(
                    'zend_lookup_class_ex(zend_string_init(ZEND_STRL("%s"), 0), NULL, 0)',
                    sprintf(
                        '%s%s%s',
                        self::NAMESPACE_SEPARATOR,
                        self::NAMESPACE_SEPARATOR,
                        $reflection->getName(),
                    ),
                );
            }

            $classNamespace = explode(self::NAMESPACE_SEPARATOR, $reflection->getNamespaceName());
        } else {
            $className = $this->classname;
            $classNamespace = explode(self::NAMESPACE_SEPARATOR, $className);
        }

        /**
         * External class, we don't know its ClassEntry in C world.
         */
        if (!$this->isInternalClass($classNamespace[0])) {
            $className = str_replace(self::NAMESPACE_SEPARATOR, self::NAMESPACE_SEPARATOR.self::NAMESPACE_SEPARATOR, $className);

            return sprintf(
                'zend_lookup_class_ex(zend_string_init(ZEND_STRL("%s"), 0), NULL, 0)',
                sprintf(
                    '%s%s%s',
                    self::NAMESPACE_SEPARATOR,
                    self::NAMESPACE_SEPARATOR,
                    $className,
                ),
            );
        }

        $className = end($classNamespace);
        array_pop($classNamespace);
        $namespace = join(self::NAMESPACE_SEPARATOR, $classNamespace);

        return (new ClassDefinition($namespace, $className))->getClassEntry();
    }

    public function isInternal(): bool
    {
        return $this->isInternal;
    }

    /**
     * Detect if start of namespace class
     * belongs to project namespace.
     *
     * @param string $className
     * @return bool
     */
    private function isInternalClass(string $className): bool
    {
        $this->isInternal = preg_match('/^'.$className.'/', $this->compilationContext->classDefinition->getNamespace()) === 1;

        return $this->isInternal;
    }
}
