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

    /**
     * Loaded via config/class-entries.php
     *
     * @var array
     */
    private array $classEntries;

    /**
     * Entry constructor.
     *
     * @param string             $className
     * @param CompilationContext $compilationContext
     */
    public function __construct(string $className, CompilationContext $compilationContext)
    {
        $this->compilationContext = $compilationContext;
        $this->classname = $this->compilationContext->getFullName($className);

        $this->classEntries = require __DIR__.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'..'.DIRECTORY_SEPARATOR.'config/class-entries.php';

        foreach ($this->classEntries as $key => $val) {
            unset($this->classEntries[$key]);
            $this->classEntries[strtolower($key)] = $val;
        }
    }

    /**
     * @return string
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function get(): string
    {
        $className = strtolower($this->classname);

        /**
         * Exclusions
         */
        if ($className === 'pdostatement') {
            $this->compilationContext->headersManager->add('kernel/main');

            return $this->compilationContext->backend->fetchClassEntry('pdostatement');
        }

        if (isset($this->classEntries[$className])) {
            if (isset($this->classEntries[$className][1])) {
                $this->compilationContext->headersManager->add($this->classEntries[$className][1]);
            }

            return $this->classEntries[$className][0];
        }

        if (class_exists($this->classname) || interface_exists($this->classname)) {
            $reflection = new ReflectionClass($this->classname);
            $className = $reflection->getName();

            /**
             * Check if class is defined internally by an extension, or the core.
             */
            if ($reflection->isInternal()) {
                return sprintf(
                    'zephir_get_internal_ce(SL("%s"))',
                    str_replace(self::NAMESPACE_SEPARATOR, self::NAMESPACE_SEPARATOR.self::NAMESPACE_SEPARATOR, strtolower($reflection->getName())),
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
        if ($classNamespace[0] === '' || !$this->isInternalClass($classNamespace[0])) {
            $className = str_replace(self::NAMESPACE_SEPARATOR, self::NAMESPACE_SEPARATOR.self::NAMESPACE_SEPARATOR, strtolower($className));

            return sprintf(
                //'zend_lookup_class_ex(zend_string_init(ZEND_STRL("%s"), 0), NULL, 0)',
                'zephir_get_internal_ce(SL("%s"))',
                $className,
            );
        }

        $className = end($classNamespace);
        array_pop($classNamespace);
        $namespace = implode(self::NAMESPACE_SEPARATOR, $classNamespace);

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
     *
     * @return bool
     */
    private function isInternalClass(string $className): bool
    {
        $this->isInternal = preg_match('/^'.$className.'/', $this->compilationContext->classDefinition->getNamespace()) === 1;

        return $this->isInternal;
    }
}
