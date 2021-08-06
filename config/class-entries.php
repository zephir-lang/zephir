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

/**
 * PHP Class Name => [Zend Class Entry, header-file-path (optional)]
 */
return [
    /**
     * SPL
     */
    'ArrayObject'                       => ['spl_ce_ArrayObject', 'ext/spl/spl_array'],
    'ArrayIterator'                     => ['spl_ce_ArrayIterator', 'ext/spl/spl_array'],
    'RecursiveArrayIterator'            => ['spl_ce_RecursiveArrayIterator', 'ext/spl/spl_array'],

    'SplFileInfo'                       => ['spl_ce_SplFileInfo', 'ext/spl/spl_directory'],
    'DirectoryIterator'                 => ['spl_ce_DirectoryIterator', 'ext/spl/spl_directory'],
    'FilesystemIterator'                => ['spl_ce_FilesystemIterator', 'ext/spl/spl_directory'],
    'RecursiveDirectoryIterator'        => ['spl_ce_RecursiveDirectoryIterator', 'ext/spl/spl_directory'],
    'GlobIterator'                      => ['spl_ce_GlobIterator', 'ext/spl/spl_directory'],
    'SplFileObject'                     => ['spl_ce_SplFileObject', 'ext/spl/spl_directory'],
    'SplTempFileObject'                 => ['spl_ce_SplTempFileObject', 'ext/spl/spl_directory'],

    'SplDoublyLinkedList'               => ['spl_ce_SplDoublyLinkedList', 'ext/spl/spl_dllist'],
    'SplQueue'                          => ['spl_ce_SplQueue', 'ext/spl/spl_dllist'],
    'SplStack'                          => ['spl_ce_SplStack', 'ext/spl/spl_dllist'],

    'LogicException'                    => ['spl_ce_LogicException', 'ext/spl/spl_exceptions'],
    'BadFunctionCallException'          => ['spl_ce_BadFunctionCallException', 'ext/spl/spl_exceptions'],
    'BadMethodCallException'            => ['spl_ce_BadMethodCallException', 'ext/spl/spl_exceptions'],
    'DomainException'                   => ['spl_ce_DomainException', 'ext/spl/spl_exceptions'],
    'InvalidArgumentException'          => ['spl_ce_InvalidArgumentException', 'ext/spl/spl_exceptions'],
    'LengthException'                   => ['spl_ce_LengthException', 'ext/spl/spl_exceptions'],
    'OutOfRangeException'               => ['spl_ce_OutOfRangeException', 'ext/spl/spl_exceptions'],
    'RuntimeException'                  => ['spl_ce_RuntimeException', 'ext/spl/spl_exceptions'],
    'OutOfBoundsException'              => ['spl_ce_OutOfBoundsException', 'ext/spl/spl_exceptions'],
    'OverflowException'                 => ['spl_ce_OverflowException', 'ext/spl/spl_exceptions'],
    'RangeException'                    => ['spl_ce_RangeException', 'ext/spl/spl_exceptions'],
    'UnderflowException'                => ['spl_ce_UnderflowException', 'ext/spl/spl_exceptions'],
    'UnexpectedValueException'          => ['spl_ce_UnexpectedValueException', 'ext/spl/spl_exceptions'],

    'SplFixedArray'                     => ['spl_ce_SplFixedArray', 'ext/spl/spl_fixedarray'],

    'SplHeap'                           => ['spl_ce_SplHeap', 'ext/spl/spl_heap'],
    'SplMinHeap'                        => ['spl_ce_SplMinHeap', 'ext/spl/spl_heap'],
    'SplMaxHeap'                        => ['spl_ce_SplMaxHeap', 'ext/spl/spl_heap'],
    'SplPriorityQueue'                  => ['spl_ce_SplPriorityQueue', 'ext/spl/spl_heap'],

    'AppendIterator'                    => ['spl_ce_AppendIterator', 'ext/spl/spl_iterators'],
    'CachingIterator'                   => ['spl_ce_CachingIterator', 'ext/spl/spl_iterators'],
    'CallbackFilterIterator'            => ['spl_ce_CallbackFilterIterator', 'ext/spl/spl_iterators'],
    'EmptyIterator'                     => ['spl_ce_EmptyIterator', 'ext/spl/spl_iterators'],
    'FilterIterator'                    => ['spl_ce_FilterIterator', 'ext/spl/spl_iterators'],
    'InfiniteIterator'                  => ['spl_ce_InfiniteIterator', 'ext/spl/spl_iterators'],
    'IteratorIterator'                  => ['spl_ce_IteratorIterator', 'ext/spl/spl_iterators'],
    'LimitIterator'                     => ['spl_ce_LimitIterator', 'ext/spl/spl_iterators'],
    'NoRewindIterator'                  => ['spl_ce_NoRewindIterator', 'ext/spl/spl_iterators'],
    'OuterIterator'                     => ['spl_ce_OuterIterator', 'ext/spl/spl_iterators'],
    'ParentIterator'                    => ['spl_ce_ParentIterator', 'ext/spl/spl_iterators'],
    'RecursiveCachingIterator'          => ['spl_ce_RecursiveCachingIterator', 'ext/spl/spl_iterators'],
    'RecursiveCallbackFilterIterator'   => ['spl_ce_RecursiveCallbackFilterIterator', 'ext/spl/spl_iterators'],
    'RecursiveFilterIterator'           => ['spl_ce_RecursiveFilterIterator', 'ext/spl/spl_iterators'],
    'RecursiveIterator'                 => ['spl_ce_RecursiveIterator', 'ext/spl/spl_iterators'],
    'RecursiveIteratorIterator'         => ['spl_ce_RecursiveIteratorIterator', 'ext/spl/spl_iterators'],
    'RecursiveRegexIterator'            => ['spl_ce_RecursiveRegexIterator', 'ext/spl/spl_iterators'],
    'RecursiveTreeIterator'             => ['spl_ce_RecursiveTreeIterator', 'ext/spl/spl_iterators'],
    'RegexIterator'                     => ['spl_ce_RegexIterator', 'ext/spl/spl_iterators'],
    'SeekableIterator'                  => ['spl_ce_SeekableIterator', 'ext/spl/spl_iterators'],

    'SplObserver'                       => ['spl_ce_SplObserver', 'ext/spl/spl_observer'],
    'SplSubject'                        => ['spl_ce_SplSubject', 'ext/spl/spl_observer'],
    'SplObjectStorage'                  => ['spl_ce_SplObjectStorage', 'ext/spl/spl_observer'],
    'MultipleIterator'                  => ['spl_ce_MultipleIterator', 'ext/spl/spl_observer'],

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
