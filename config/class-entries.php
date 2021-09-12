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
 * List of Class Entries and respective headers.
 *
 * These Class Entries can't be detected dynamically and must be
 * specified with exact class entry name and its header where it
 * has been declared.
 * Most or near all of them are from bundled PHP extensions.
 *
 * Full list of bundled PHP extensions can be found here:
 * @link https://github.com/php/php-src/tree/master/ext
 *
 * If Class is not located in this list, or it is from an external
 * extension, then Zephir will generate Zend C code for dynamic CE
 * detection via full namespace class name.
 *
 * Example:
 * ```
 * Z_PARAM_OBJECT_OF_CLASS(link, zephir_get_internal_ce(SL("psr\\link\\linkinterface")))
 * ```
 *
 * PHP Class Name => [Zend Class Entry, header-file-path (optional)]
 */
return [
    /**
     * cURL
     *
     * @link https://github.com/php/php-src/tree/master/ext/curl
     */
    'CurlHandle'                            => ['curl_ce', 'ext/curl/php_curl'],
    'CurlShareHandle'                       => ['curl_share_ce', 'ext/curl/php_curl'],
    'CurlMultiHandle'                       => ['curl_multi_ce', 'ext/curl/php_curl'],
    'CURLFile'                              => ['curl_CURLFile_class', 'ext/curl/php_curl'],
    'CURLStringFile'                        => ['curl_CURLStringFile_class', 'ext/curl/php_curl'],

    /**
     * Date
     *
     * @link https://github.com/php/php-src/tree/master/ext/date
     */
    'DateTimeInterface'                     => ['php_date_get_interface_ce()', 'ext/date/php_date'],
    'DateTime'                              => ['php_date_get_date_ce()', 'ext/date/php_date'],
    'DateTimeImmutable'                     => ['php_date_get_immutable_ce()', 'ext/date/php_date'],
    'DateTimezone'                          => ['php_date_get_timezone_ce()', 'ext/date/php_date'],
    'DateInterval'                          => ['php_date_get_interval_ce()', 'ext/date/php_date'],
    'DatePeriod'                            => ['php_date_get_period_ce()', 'ext/date/php_date'],

    /**
     * DOM
     *
     * @link https://github.com/php/php-src/tree/master/ext/dom
     */
    'DOMNode'                               => ['dom_node_class_entry', 'ext/dom/xml_common'],

    /**
     * GMP
     *
     * @link https://github.com/php/php-src/tree/master/ext/gmp
     */
    'GMP'                                   => ['php_gmp_class_entry()', 'ext/gmp/php_gmp_int'],

    /**
     * Hash
     *
     * @link https://github.com/php/php-src/tree/master/ext/hash
     */
    'HashContext'                           => ['php_hashcontext_ce', 'ext/hash/php_hash'],

    /**
     * Intl
     *
     * @link https://github.com/php/php-src/tree/master/ext/intl
     */
    'IntlException'                         => ['IntlException_ce_ptr', 'ext/intl/intl_common'],

    'IntlBreakIterator'                     => ['BreakIterator_ce_ptr', 'ext/intl/breakiterator/breakiterator_class'],
    'IntlRuleBasedBreakIterator'            => ['RuleBasedBreakIterator_ce_ptr', 'ext/intl/breakiterator/breakiterator_class'],

    'IntlCalendar'                          => ['Calendar_ce_ptr', 'ext/intl/calendar/calendar_class'],
    'IntlGregorianCalendar'                 => ['GregorianCalendar_ce_ptr', 'ext/intl/calendar/calendar_class'],

    'Collator'                              => ['GregorianCalendar_ce_ptr', 'ext/intl/collator/collator_class'],

    'IntlIterator'                          => ['IntlIterator_ce_ptr', 'ext/intl/common/collator_enum'],

    'IntlDateFormatter'                     => ['IntlDateFormatter_ce_ptr', 'ext/intl/dateformat/dateformat_class'],
    'IntlDatePatternGenerator'              => ['IntlDatePatternGenerator_ce_ptr', 'ext/intl/dateformat/datepatterngenerator_class'],

    'NumberFormatter'                       => ['NumberFormatter_ce_ptr', 'ext/intl/formatter/formatter_class'],

    'Locale'                                => ['Locale_ce_ptr', 'ext/intl/locale/locale_class'],

    'MessageFormatter'                      => ['MessageFormatter_ce_ptr', 'ext/intl/locale/msgformat/msgformat_class'],

    'Normalizer'                            => ['Normalizer_ce_ptr', 'ext/intl/locale/normalizer/normalizer_class'],

    'ResourceBundle'                        => ['ResourceBundle_ce_ptr', 'ext/intl/resourcebundle/resourcebundle_class'],

    'Spoofchecker'                          => ['Spoofchecker_ce_ptr', 'ext/intl/spoofchecker/spoofchecker_class'],

    'IntlTimeZone'                          => ['TimeZone_ce_ptr', 'ext/intl/timezone/timezone_class'],

    'Transliterator'                        => ['Transliterator_ce_ptr', 'ext/intl/transliterator/transliterator_class'],

    /**
     * JSON
     *
     * @link https://github.com/php/php-src/tree/master/ext/json
     */
    'JsonSerializable'                          => ['php_json_serializable_ce', 'ext/json/php_json'],

    /**
     * MySQLi
     *
     * @link https://github.com/php/php-src/tree/master/ext/mysqli
     */
    'mysqli'                                    => ['mysqli_link_class_entry', 'ext/mysqli/php_mysqli_structs'],
    'mysqli_stmt'                               => ['mysqli_stmt_class_entry', 'ext/mysqli/php_mysqli_structs'],
    'mysqli_result'                             => ['mysqli_result_class_entry', 'ext/mysqli/php_mysqli_structs'],
    'mysqli_driver'                             => ['mysqli_driver_class_entry', 'ext/mysqli/php_mysqli_structs'],
    'mysqli_warning'                            => ['mysqli_warning_class_entry', 'ext/mysqli/php_mysqli_structs'],
    'mysqli_sql_exception'                      => ['mysqli_exception_class_entry', 'ext/mysqli/php_mysqli_structs'],

    /**
     * OCI8
     *
     * @link https://github.com/php/php-src/tree/master/ext/oci8
     */
    'OCILob'                                    => ['oci_lob_class_entry_ptr', 'ext/oci8/php_oci8_int'],
    'OCICollection'                             => ['oci_coll_class_entry_ptr', 'ext/oci8/php_oci8_int'],

    /**
     * OpenSSL
     *
     * @link https://github.com/php/php-src/tree/master/ext/openssl
     */
    'OpenSSLCertificate'                        => ['php_openssl_certificate_ce', 'ext/openssl/php_openssl'],

    /**
     * PDO
     *
     * @link https://github.com/php/php-src/tree/master/ext/pdo
     */
    'PDO'                                       => ['php_pdo_get_dbh_ce()', 'ext/pdo/php_pdo_driver'],
    'PDOException'                              => ['php_pdo_get_exception()', 'ext/pdo/php_pdo_driver'],

    /**
     * Reflection
     *
     * @link https://github.com/php/php-src/tree/master/ext/reflection
     */
    'Reflector'                                 => ['reflector_ptr', 'ext/reflection/php_reflection'],
    'ReflectionException'                       => ['reflection_exception_ptr', 'ext/reflection/php_reflection'],
    'Reflection'                                => ['reflection_ptr', 'ext/reflection/php_reflection'],
    'ReflectionFunctionAbstract'                => ['reflection_function_abstract_ptr', 'ext/reflection/php_reflection'],
    'ReflectionFunction'                        => ['reflection_function_ptr', 'ext/reflection/php_reflection'],
    'ReflectionParameter'                       => ['reflection_parameter_ptr', 'ext/reflection/php_reflection'],
    'ReflectionType'                            => ['reflection_type_ptr', 'ext/reflection/php_reflection'],
    'ReflectionNamedType'                       => ['reflection_named_type_ptr', 'ext/reflection/php_reflection'],
    'ReflectionClass'                           => ['reflection_class_ptr', 'ext/reflection/php_reflection'],
    'ReflectionObject'                          => ['reflection_object_ptr', 'ext/reflection/php_reflection'],
    'ReflectionMethod'                          => ['reflection_method_ptr', 'ext/reflection/php_reflection'],
    'ReflectionProperty'                        => ['reflection_property_ptr', 'ext/reflection/php_reflection'],
    'ReflectionExtension'                       => ['reflection_extension_ptr', 'ext/reflection/php_reflection'],
    'ReflectionZendExtension'                   => ['reflection_zend_extension_ptr', 'ext/reflection/php_reflection'],
    'ReflectionReference'                       => ['reflection_reference_ptr', 'ext/reflection/php_reflection'],
    'ReflectionAttribute'                       => ['reflection_attribute_ptr', 'ext/reflection/php_reflection'],
    'ReflectionEnum'                            => ['reflection_enum_ptr', 'ext/reflection/php_reflection'],
    'ReflectionEnumUnitCase'                    => ['reflection_enum_unit_case_ptr', 'ext/reflection/php_reflection'],
    'ReflectionEnumBackedCase'                  => ['reflection_enum_backed_case_ptr', 'ext/reflection/php_reflection'],
    'ReflectionFiber'                           => ['reflection_fiber_ptr', 'ext/reflection/php_reflection'],

    /**
     * Session
     *
     * @link https://github.com/php/php-src/tree/master/ext/session
     */
    'SessionHandler'                            => ['php_session_class_entry', 'ext/session/php_session'],
    'SessionHandlerInterface'                   => ['php_session_iface_entry', 'ext/session/php_session'],
    'SessionIdInterface'                        => ['php_session_id_iface_entry', 'ext/session/php_session'],
    'SessionUpdateTimestampHandlerInterface'    => ['php_session_update_timestamp_iface_entry', 'ext/session/php_session'],

    /**
     * SimpleXML
     *
     * @link https://github.com/php/php-src/tree/master/ext/simplexml
     */
    'SimpleXMLIterator'                         => ['ce_SimpleXMLIterator', 'ext/simplexml/php_simplexml'],
    'SimpleXMLElement'                          => ['ce_SimpleXMLElement', 'ext/simplexml/php_simplexml'],

    /**
     * SOAP
     *
     * @link https://github.com/php/php-src/tree/master/ext/soap
     */
    'SoapClient'                                => ['soap_class_entry', 'ext/soap/php_soap'],
    'SoapVar'                                   => ['soap_var_class_entry', 'ext/soap/php_soap'],

    /**
     * Sockets
     */
    'Socket'                                    => ['socket_ce', 'ext/sockets/php_sockets'],

    /**
     * SPL
     */
    'ArrayObject'                               => ['spl_ce_ArrayObject', 'ext/spl/spl_array'],
    'ArrayIterator'                             => ['spl_ce_ArrayIterator', 'ext/spl/spl_array'],
    'RecursiveArrayIterator'                    => ['spl_ce_RecursiveArrayIterator', 'ext/spl/spl_array'],

    'SplFileInfo'                               => ['spl_ce_SplFileInfo', 'ext/spl/spl_directory'],
    'DirectoryIterator'                         => ['spl_ce_DirectoryIterator', 'ext/spl/spl_directory'],
    'FilesystemIterator'                        => ['spl_ce_FilesystemIterator', 'ext/spl/spl_directory'],
    'RecursiveDirectoryIterator'                => ['spl_ce_RecursiveDirectoryIterator', 'ext/spl/spl_directory'],
    'GlobIterator'                              => ['spl_ce_GlobIterator', 'ext/spl/spl_directory'],
    'SplFileObject'                             => ['spl_ce_SplFileObject', 'ext/spl/spl_directory'],
    'SplTempFileObject'                         => ['spl_ce_SplTempFileObject', 'ext/spl/spl_directory'],

    'SplDoublyLinkedList'                       => ['spl_ce_SplDoublyLinkedList', 'ext/spl/spl_dllist'],
    'SplQueue'                                  => ['spl_ce_SplQueue', 'ext/spl/spl_dllist'],
    'SplStack'                                  => ['spl_ce_SplStack', 'ext/spl/spl_dllist'],

    'LogicException'                            => ['spl_ce_LogicException', 'ext/spl/spl_exceptions'],
    'BadFunctionCallException'                  => ['spl_ce_BadFunctionCallException', 'ext/spl/spl_exceptions'],
    'BadMethodCallException'                    => ['spl_ce_BadMethodCallException', 'ext/spl/spl_exceptions'],
    'DomainException'                           => ['spl_ce_DomainException', 'ext/spl/spl_exceptions'],
    'InvalidArgumentException'                  => ['spl_ce_InvalidArgumentException', 'ext/spl/spl_exceptions'],
    'LengthException'                           => ['spl_ce_LengthException', 'ext/spl/spl_exceptions'],
    'OutOfRangeException'                       => ['spl_ce_OutOfRangeException', 'ext/spl/spl_exceptions'],
    'RuntimeException'                          => ['spl_ce_RuntimeException', 'ext/spl/spl_exceptions'],
    'OutOfBoundsException'                      => ['spl_ce_OutOfBoundsException', 'ext/spl/spl_exceptions'],
    'OverflowException'                         => ['spl_ce_OverflowException', 'ext/spl/spl_exceptions'],
    'RangeException'                            => ['spl_ce_RangeException', 'ext/spl/spl_exceptions'],
    'UnderflowException'                        => ['spl_ce_UnderflowException', 'ext/spl/spl_exceptions'],
    'UnexpectedValueException'                  => ['spl_ce_UnexpectedValueException', 'ext/spl/spl_exceptions'],

    'SplFixedArray'                             => ['spl_ce_SplFixedArray', 'ext/spl/spl_fixedarray'],

    'SplHeap'                                   => ['spl_ce_SplHeap', 'ext/spl/spl_heap'],
    'SplMinHeap'                                => ['spl_ce_SplMinHeap', 'ext/spl/spl_heap'],
    'SplMaxHeap'                                => ['spl_ce_SplMaxHeap', 'ext/spl/spl_heap'],
    'SplPriorityQueue'                          => ['spl_ce_SplPriorityQueue', 'ext/spl/spl_heap'],

    'AppendIterator'                            => ['spl_ce_AppendIterator', 'ext/spl/spl_iterators'],
    'CachingIterator'                           => ['spl_ce_CachingIterator', 'ext/spl/spl_iterators'],
    'CallbackFilterIterator'                    => ['spl_ce_CallbackFilterIterator', 'ext/spl/spl_iterators'],
    'EmptyIterator'                             => ['spl_ce_EmptyIterator', 'ext/spl/spl_iterators'],
    'FilterIterator'                            => ['spl_ce_FilterIterator', 'ext/spl/spl_iterators'],
    'InfiniteIterator'                          => ['spl_ce_InfiniteIterator', 'ext/spl/spl_iterators'],
    'IteratorIterator'                          => ['spl_ce_IteratorIterator', 'ext/spl/spl_iterators'],
    'LimitIterator'                             => ['spl_ce_LimitIterator', 'ext/spl/spl_iterators'],
    'NoRewindIterator'                          => ['spl_ce_NoRewindIterator', 'ext/spl/spl_iterators'],
    'OuterIterator'                             => ['spl_ce_OuterIterator', 'ext/spl/spl_iterators'],
    'ParentIterator'                            => ['spl_ce_ParentIterator', 'ext/spl/spl_iterators'],
    'RecursiveCachingIterator'                  => ['spl_ce_RecursiveCachingIterator', 'ext/spl/spl_iterators'],
    'RecursiveCallbackFilterIterator'           => ['spl_ce_RecursiveCallbackFilterIterator', 'ext/spl/spl_iterators'],
    'RecursiveFilterIterator'                   => ['spl_ce_RecursiveFilterIterator', 'ext/spl/spl_iterators'],
    'RecursiveIterator'                         => ['spl_ce_RecursiveIterator', 'ext/spl/spl_iterators'],
    'RecursiveIteratorIterator'                 => ['spl_ce_RecursiveIteratorIterator', 'ext/spl/spl_iterators'],
    'RecursiveRegexIterator'                    => ['spl_ce_RecursiveRegexIterator', 'ext/spl/spl_iterators'],
    'RecursiveTreeIterator'                     => ['spl_ce_RecursiveTreeIterator', 'ext/spl/spl_iterators'],
    'RegexIterator'                             => ['spl_ce_RegexIterator', 'ext/spl/spl_iterators'],
    'SeekableIterator'                          => ['spl_ce_SeekableIterator', 'ext/spl/spl_iterators'],

    'SplObserver'                               => ['spl_ce_SplObserver', 'ext/spl/spl_observer'],
    'SplSubject'                                => ['spl_ce_SplSubject', 'ext/spl/spl_observer'],
    'SplObjectStorage'                          => ['spl_ce_SplObjectStorage', 'ext/spl/spl_observer'],
    'MultipleIterator'                          => ['spl_ce_MultipleIterator', 'ext/spl/spl_observer'],

    /**
     * Standard
     */
    'AssertionError'                            => ['assertion_error_ce', 'ext/standard/php_assert'],

    '__PHP_Incomplete_Class'                    => ['php_ce_incomplete_class', 'ext/standard/php_incomplete_class'],

    /**
     * Closures
     */
    'Closure'                                   => ['zend_ce_closure', 'Zend/zend_closures'],

    /**
     * Zend exceptions
     */
    'Throwable'                                 => ['zend_ce_throwable'],
    'Exception'                                 => ['zend_ce_exception'],
    'ErrorException'                            => ['zend_ce_error_exception'],
    'Error'                                     => ['zend_ce_error'],
    'CompileError'                              => ['zend_ce_compile_error'],
    'ParseError'                                => ['zend_ce_parse_error'],
    'TypeError'                                 => ['zend_ce_type_error'],
    'ArgumentCountError'                        => ['zend_ce_argument_count_error'],
    'ValueError'                                => ['zend_ce_value_error'],
    'ArithmeticError'                           => ['zend_ce_arithmetic_error'],
    'DivisionByZeroError'                       => ['zend_ce_division_by_zero_error'],
    'UnhandledMatchError'                       => ['zend_ce_unhandled_match_error'],

    /**
     * Zend interfaces (Zend/zend_interfaces.h)
     */
    'Traversable'                               => ['zend_ce_traversable'],
    'IteratorAggregate'                         => ['zend_ce_aggregate'],
    'Iterator'                                  => ['zend_ce_iterator'],
    'ArrayAccess'                               => ['zend_ce_arrayaccess'],
    'Serializable'                              => ['zend_ce_serializable'],
    'Countable'                                 => ['zend_ce_countable'],
    'Stringable'                                => ['zend_ce_stringable'],

    'stdClass'                                  => ['zend_standard_class_def'],
];
