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

    private const EXT_SPL_ARRAY = 'ext/spl/spl_array';
    private const EXT_SPL_DIRECTORY = 'ext/spl/spl_directory';
    private const EXT_SPL_DLLIST = 'ext/spl/spl_dllist';
    private const EXT_SPL_EXCEPTIONS = 'ext/spl/spl_exceptions';
    private const EXT_SPL_FIXEDARRAY = 'ext/spl/spl_fixedarray';
    private const EXT_SPL_HEAP = 'ext/spl/spl_heap';
    private const EXT_SPL_ITERATORS = 'ext/spl/spl_iterators';
    private const EXT_SPL_OBSERVER = 'ext/spl/spl_observer';

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

    private array $classEntries = [
        /**
         * SPL
         */
        'ArrayObject'                       => ['spl_ce_ArrayObject', self::EXT_SPL_ARRAY],
        'ArrayIterator'                     => ['spl_ce_ArrayIterator', self::EXT_SPL_ARRAY],
        'RecursiveArrayIterator'            => ['spl_ce_RecursiveArrayIterator', self::EXT_SPL_ARRAY],

        'SplFileInfo'                       => ['spl_ce_SplFileInfo', self::EXT_SPL_DIRECTORY],
        'DirectoryIterator'                 => ['spl_ce_DirectoryIterator', self::EXT_SPL_DIRECTORY],
        'FilesystemIterator'                => ['spl_ce_FilesystemIterator', self::EXT_SPL_DIRECTORY],
        'RecursiveDirectoryIterator'        => ['spl_ce_RecursiveDirectoryIterator', self::EXT_SPL_DIRECTORY],
        'GlobIterator'                      => ['spl_ce_GlobIterator', self::EXT_SPL_DIRECTORY],
        'SplFileObject'                     => ['spl_ce_SplFileObject', self::EXT_SPL_DIRECTORY],
        'SplTempFileObject'                 => ['spl_ce_SplTempFileObject', self::EXT_SPL_DIRECTORY],

        'SplDoublyLinkedList'               => ['spl_ce_SplDoublyLinkedList', self::EXT_SPL_DLLIST],
        'SplQueue'                          => ['spl_ce_SplQueue', self::EXT_SPL_DLLIST],
        'SplStack'                          => ['spl_ce_SplStack', self::EXT_SPL_DLLIST],

        'LogicException'                    => ['spl_ce_LogicException', self::EXT_SPL_EXCEPTIONS],
        'BadFunctionCallException'          => ['spl_ce_BadFunctionCallException', self::EXT_SPL_EXCEPTIONS],
        'BadMethodCallException'            => ['spl_ce_BadMethodCallException', self::EXT_SPL_EXCEPTIONS],
        'DomainException'                   => ['spl_ce_DomainException', self::EXT_SPL_EXCEPTIONS],
        'InvalidArgumentException'          => ['spl_ce_InvalidArgumentException', self::EXT_SPL_EXCEPTIONS],
        'LengthException'                   => ['spl_ce_LengthException', self::EXT_SPL_EXCEPTIONS],
        'OutOfRangeException'               => ['spl_ce_OutOfRangeException', self::EXT_SPL_EXCEPTIONS],
        'RuntimeException'                  => ['spl_ce_RuntimeException', self::EXT_SPL_EXCEPTIONS],
        'OutOfBoundsException'              => ['spl_ce_OutOfBoundsException', self::EXT_SPL_EXCEPTIONS],
        'OverflowException'                 => ['spl_ce_OverflowException', self::EXT_SPL_EXCEPTIONS],
        'RangeException'                    => ['spl_ce_RangeException', self::EXT_SPL_EXCEPTIONS],
        'UnderflowException'                => ['spl_ce_UnderflowException', self::EXT_SPL_EXCEPTIONS],
        'UnexpectedValueException'          => ['spl_ce_UnexpectedValueException', self::EXT_SPL_EXCEPTIONS],

        'SplFixedArray'                     => ['spl_ce_SplFixedArray', self::EXT_SPL_FIXEDARRAY],

        'SplHeap'                           => ['spl_ce_SplHeap', self::EXT_SPL_HEAP],
        'SplMinHeap'                        => ['spl_ce_SplMinHeap', self::EXT_SPL_HEAP],
        'SplMaxHeap'                        => ['spl_ce_SplMaxHeap', self::EXT_SPL_HEAP],
        'SplPriorityQueue'                  => ['spl_ce_SplPriorityQueue', self::EXT_SPL_HEAP],

        'AppendIterator'                    => ['spl_ce_AppendIterator', self::EXT_SPL_ITERATORS],
        'CachingIterator'                   => ['spl_ce_CachingIterator', self::EXT_SPL_ITERATORS],
        'CallbackFilterIterator'            => ['spl_ce_CallbackFilterIterator', self::EXT_SPL_ITERATORS],
        'EmptyIterator'                     => ['spl_ce_EmptyIterator', self::EXT_SPL_ITERATORS],
        'FilterIterator'                    => ['spl_ce_FilterIterator', self::EXT_SPL_ITERATORS],
        'InfiniteIterator'                  => ['spl_ce_InfiniteIterator', self::EXT_SPL_ITERATORS],
        'IteratorIterator'                  => ['spl_ce_IteratorIterator', self::EXT_SPL_ITERATORS],
        'LimitIterator'                     => ['spl_ce_LimitIterator', self::EXT_SPL_ITERATORS],
        'NoRewindIterator'                  => ['spl_ce_NoRewindIterator', self::EXT_SPL_ITERATORS],
        'OuterIterator'                     => ['spl_ce_OuterIterator', self::EXT_SPL_ITERATORS],
        'ParentIterator'                    => ['spl_ce_ParentIterator', self::EXT_SPL_ITERATORS],
        'RecursiveCachingIterator'          => ['spl_ce_RecursiveCachingIterator', self::EXT_SPL_ITERATORS],
        'RecursiveCallbackFilterIterator'   => ['spl_ce_RecursiveCallbackFilterIterator', self::EXT_SPL_ITERATORS],
        'RecursiveFilterIterator'           => ['spl_ce_RecursiveFilterIterator', self::EXT_SPL_ITERATORS],
        'RecursiveIterator'                 => ['spl_ce_RecursiveIterator', self::EXT_SPL_ITERATORS],
        'RecursiveIteratorIterator'         => ['spl_ce_RecursiveIteratorIterator', self::EXT_SPL_ITERATORS],
        'RecursiveRegexIterator'            => ['spl_ce_RecursiveRegexIterator', self::EXT_SPL_ITERATORS],
        'RecursiveTreeIterator'             => ['spl_ce_RecursiveTreeIterator', self::EXT_SPL_ITERATORS],
        'RegexIterator'                     => ['spl_ce_RegexIterator', self::EXT_SPL_ITERATORS],
        'SeekableIterator'                  => ['spl_ce_SeekableIterator', self::EXT_SPL_ITERATORS],

        'SplObserver'                       => ['spl_ce_SplObserver', self::EXT_SPL_OBSERVER],
        'SplSubject'                        => ['spl_ce_SplSubject', self::EXT_SPL_OBSERVER],
        'SplObjectStorage'                  => ['spl_ce_SplObjectStorage', self::EXT_SPL_OBSERVER],
        'MultipleIterator'                  => ['spl_ce_MultipleIterator', self::EXT_SPL_OBSERVER],

        /**
         * Session
         */
        'SessionHandlerInterface'           => ['php_session_iface_entry', 'ext/session/php_session'],

        /**
         * Date
         */
        'DateTimeInterface'                 => ['php_date_get_interface_ce()', 'ext/date/php_date'],
        'DateTime'                          => ['php_date_get_date_ce()', 'ext/date/php_date'],
        'DateTimeImmutable'                 => ['php_date_get_immutable_ce()', 'ext/date/php_date'],
        'DateTimezone'                      => ['php_date_get_timezone_ce()', 'ext/date/php_date'],
        'DateInterval'                      => ['php_date_get_interval_ce()', 'ext/date/php_date'],
        'DatePeriod'                        => ['php_date_get_period_ce()', 'ext/date/php_date'],

        /**
         * Closures
         */
        'Closure'                           => ['zend_ce_closure', 'Zend/zend_closures'],

        /**
         * Zend exceptions
         */
        'Throwable'                         => ['zend_ce_throwable'],
        'Exception'                         => ['zend_ce_exception'],
        'ErrorException'                    => ['zend_ce_error_exception'],
        'Error'                             => ['zend_ce_error'],
        'CompileError'                      => ['zend_ce_compile_error'],
        'ParseError'                        => ['zend_ce_parse_error'],
        'TypeError'                         => ['zend_ce_type_error'],
        'ArgumentCountError'                => ['zend_ce_argument_count_error'],
        'ValueError'                        => ['zend_ce_value_error'],
        'ArithmeticError'                   => ['zend_ce_arithmetic_error'],
        'DivisionByZeroError'               => ['zend_ce_division_by_zero_error'],
        'UnhandledMatchError'               => ['zend_ce_unhandled_match_error'],

        /**
         * Zend interfaces (Zend/zend_interfaces.h)
         */
        'Traversable'                       => ['zend_ce_traversable'],
        'IteratorAggregate'                 => ['zend_ce_aggregate'],
        'Iterator'                          => ['zend_ce_iterator'],
        'ArrayAccess'                       => ['zend_ce_arrayaccess'],
        'Serializable'                      => ['zend_ce_serializable'],
        'Countable'                         => ['zend_ce_countable'],
        'Stringable'                        => ['zend_ce_stringable'],

        /**
         * PDO
         */
        'PDO'                               => ['php_pdo_get_dbh_ce()', 'ext/pdo/php_pdo_driver'],
        'PDOException'                      => ['php_pdo_get_exception()', 'ext/pdo/php_pdo_driver'],

        'stdClass'                          => ['zend_standard_class_def'],
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

        foreach ($this->classEntries as $key => $val) {
            unset($this->classEntries[$key]);
            $this->classEntries[strtolower($key)] = $val;
        }
    }

    /**
     * @return string
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

            return $this->compilationContext->backend->fetchClassEntry('pdostatement');;
        }

        if (isset($this->classEntries[$className])) {
            if (isset($this->classEntries[$className][1])) {
                $this->compilationContext->headersManager->add($this->classEntries[$className][1]);
            }

            return $this->classEntries[$className][0];
        }

        if (class_exists($this->classname)) {
            $reflection = new ReflectionClass($this->classname);
            $className = $reflection->getName();

            /**
             * Check if class is defined internally by an extension, or the core.
             */
            if ($reflection->isInternal()) {
                return sprintf(
                    'zephir_get_internal_ce(SL("%s"))',
                    strtolower($reflection->getName()),
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
            $className = str_replace(self::NAMESPACE_SEPARATOR, self::NAMESPACE_SEPARATOR.self::NAMESPACE_SEPARATOR, strtolower($className));

            return sprintf(
                //'zend_lookup_class_ex(zend_string_init(ZEND_STRL("%s"), 0), NULL, 0)',
                'zephir_get_internal_ce(SL("%s"))',
                $className,
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
