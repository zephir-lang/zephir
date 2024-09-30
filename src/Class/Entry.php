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

namespace Zephir\Class;

use ReflectionClass;
use Zephir\Class\Definition\Definition;
use Zephir\CompilationContext;
use Zephir\Exception;

use function array_pop;
use function class_exists;
use function end;
use function explode;
use function implode;
use function interface_exists;
use function preg_match;
use function sprintf;
use function str_replace;
use function strtolower;

use const DIRECTORY_SEPARATOR;

/**
 * Zend Class Entry detector
 */
class Entry
{
    public const NAMESPACE_SEPARATOR = '\\';
    /**
     * Loaded via config/class-entries.php
     */
    private array $classEntries;
    /**
     * As it was passed: partially or fully.
     */
    private string $classname;
    private bool   $isInternal = false;

    public function __construct(string $className, private CompilationContext $compilationContext)
    {
        $this->classname    = $this->compilationContext->getFullName($className);
        $this->classEntries = require __DIR__ . DIRECTORY_SEPARATOR . '..' . DIRECTORY_SEPARATOR . '..' . DIRECTORY_SEPARATOR . 'config/class-entries.php';

        foreach ($this->classEntries as $key => $val) {
            unset($this->classEntries[$key]);
            $this->classEntries[strtolower($key)] = $val;
        }
    }

    /**
     * Prepares a class name to be used as a C-string.
     */
    public static function escape(string $className): string
    {
        return str_replace(static::NAMESPACE_SEPARATOR, '\\\\', $className);
    }

    /**
     * @throws Exception
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
            $className  = $reflection->getName();

            /**
             * Check if class is defined internally by an extension, or the core.
             */
            if ($reflection->isInternal()) {
                return sprintf(
                    'zephir_get_internal_ce(SL("%s"))',
                    str_replace(
                        self::NAMESPACE_SEPARATOR,
                        self::NAMESPACE_SEPARATOR . self::NAMESPACE_SEPARATOR,
                        strtolower($reflection->getName())
                    ),
                );
            }

            $classNamespace = explode(self::NAMESPACE_SEPARATOR, $reflection->getName());
        } else {
            $className      = $this->classname;
            $classNamespace = explode(self::NAMESPACE_SEPARATOR, $className);
        }

        /**
         * External class, we don't know its ClassEntry in C world.
         */
        if ($classNamespace[0] === '' || !$this->isInternalClass($classNamespace[0])) {
            $className = str_replace(
                self::NAMESPACE_SEPARATOR,
                self::NAMESPACE_SEPARATOR . self::NAMESPACE_SEPARATOR,
                strtolower($className)
            );

            return sprintf(
                'zephir_get_internal_ce(SL("%s"))',
                $className,
            );
        }

        $className = end($classNamespace);
        array_pop($classNamespace);
        $namespace = implode(self::NAMESPACE_SEPARATOR, $classNamespace);

        return (new Definition($namespace, $className))->getClassEntry();
    }

    public function isInternal(): bool
    {
        return $this->isInternal;
    }

    /**
     * Detect if start of namespace class belongs to project namespace.
     */
    private function isInternalClass(string $className): bool
    {
        $this->isInternal = preg_match(
            '/^' . $className . '/',
            $this->compilationContext->classDefinition->getNamespace()
        ) === 1;

        return $this->isInternal;
    }
}
