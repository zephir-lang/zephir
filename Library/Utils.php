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

namespace Zephir;

/**
 * Utils
 *
 * Utility functions
 */
class Utils
{
    /**
     * Prepares a class name to be used as a C-string
     *
     * @param string $className
     * @return string
     */
    public static function escapeClassName($className)
    {
        return str_replace('\\', '\\\\', $className);
    }

    /**
     * Prepares a string to be used as a C-string
     *
     * @param string $str
     * @param bool $escapeSlash
     * @return string
     */
    public static function addSlashes($str, $escapeSlash = false)
    {
        $newstr = "";
        $after = null;
        $before = null;
        $length = strlen($str);
        for ($i = 0; $i < $length; $i++) {
            $ch = substr($str, $i, 1);
            if ($i != ($length -1)) {
                $after = substr($str, $i + 1, 1);
            } else {
                $after = null;
            }

            switch ($ch) {
                case '"':
                    $newstr .= "\\" . '"';
                    break;
                case "\n":
                    $newstr .= "\\" . 'n';
                    break;
                case "\t":
                    $newstr .= "\\" . 't';
                    break;
                case "\r":
                    $newstr .= "\\" . 'r';
                    break;
                case "\v":
                    $newstr .= "\\" . 'v';
                    break;
                case '\\':
                    switch ($after) {
                        case "n":
                        case "v":
                        case "t":
                        case "r":
                        case '"':
                        case "\\":
                            $newstr .= $ch . $after;
                            $i++;
                            $before = null;
                            continue;
                        default:
                            $newstr .= "\\\\";
                            break;
                    }
                    break;
                default:
                    $newstr .= $ch;
            }
            $before = $ch;
        }
        return $newstr;
    }

    /**
     * Camelize a string
     *
     * @param       string $str
     * @return      string
     */
    public static function camelize($str)
    {
        return str_replace(' ', '', ucwords(str_replace('_', ' ', $str)));
    }

    /**
     * Checks if the content of the file on the disk is the same as the content.
     *
     * @param $content
     * @param $path
     * @return bool
     */
    public static function checkAndWriteIfNeeded($content, $path)
    {
        if (file_exists($path)) {
            $contentMd5 = md5($content);
            $existingMd5 = md5_file($path);

            if ($contentMd5 != $existingMd5) {
                file_put_contents($path, $content);
                return true;
            }
        } else {
            file_put_contents($path, $content);
            return true;
        }

        return false;
    }

    /**
     * Transform class/interface name to FQN format
     *
     * @param string $className
     * @param string $currentNamespace
     * @param AliasManager $aliasManager
     * @return string
     */
    public static function getFullName($className, $currentNamespace, AliasManager $aliasManager = null)
    {
        if (!is_string($className)) {
            throw new \InvalidArgumentException('Class name must be a string ' . print_r($className, true));
        }

        if ($className[0] !== '\\') {
            // If class/interface name not begin with \ maybe a alias or a sub-namespace
            $firstSepPos = strpos($className, '\\');
            if (false !== $firstSepPos) {
                $baseName = substr($className, 0, $firstSepPos);
                if ($aliasManager->isAlias($baseName)) {
                    return $aliasManager->getAlias($baseName) . '\\' . substr($className, $firstSepPos + 1);
                }
            } else {
                if ($aliasManager->isAlias($className)) {
                    return $aliasManager->getAlias($className);
                }
            }

            // Relative class/interface name
            if ($currentNamespace) {
                return $currentNamespace . '\\' . $className;
            } else {
                return $className;
            }
        }

        // Absolute class/interface name
        return substr($className, 1);
    }

    /**
     * Convert Class/Interface name to C ClassEntry
     *
     * @param  string $className
     * @param  CompilationContext $compilationContext
     * @param  boolean $check
     * @return string
     * @throws CompilerException
     */
    public static function getClassEntryByClassName($className, CompilationContext $compilationContext, $check = true)
    {
        switch (strtolower($className)) {

            /**
             * Zend classes
             */
            case 'exception':
                $classEntry = 'zend_exception_get_default(TSRMLS_C)';
                break;

            /**
             * Zend interfaces (Zend/zend_interfaces.h)
             */
            case 'iterator':
                $classEntry = 'zend_ce_iterator';
                break;

            case 'arrayaccess':
                $classEntry = 'zend_ce_arrayaccess';
                break;

            case 'serializable':
                $classEntry = 'zend_ce_serializable';
                break;

            case 'iteratoraggregate':
                $classEntry = 'zend_ce_aggregate';
                break;

            /**
             * SPL Exceptions
             */
            case 'logicexception':
                $classEntry = 'spl_ce_LogicException';
                break;

            case 'badfunctioncallexception':
                $classEntry = 'spl_ce_BadFunctionCallException';
                break;

            case 'badmethodcallexception':
                $classEntry = 'spl_ce_BadMethodCallException';
                break;

            case 'domainexception':
                $classEntry = 'spl_ce_DomainException';
                break;

            case 'invalidargumentexception':
                $classEntry = 'spl_ce_InvalidArgumentException';
                break;

            case 'lengthexception':
                $classEntry = 'spl_ce_LengthException';
                break;

            case 'outofrangeexception':
                $classEntry = 'spl_ce_OutOfRangeException';
                break;

            case 'runtimeexception':
                $classEntry = 'spl_ce_RuntimeException';
                break;

            case 'outofboundsexception':
                $classEntry = 'spl_ce_OutOfBoundsException';
                break;

            case 'overflowexception':
                $classEntry = 'spl_ce_OverflowException';
                break;

            case 'rangeexception':
                $classEntry = 'spl_ce_RangeException';
                break;

            case 'underflowexception':
                $classEntry = 'spl_ce_UnderflowException';
                break;

            case 'unexpectedvalueexception':
                $classEntry = 'spl_ce_UnexpectedValueException';
                break;

            /**
             * SPL Iterators Interfaces (spl/spl_iterators.h)
             */
            case 'recursiveiterator':
                $classEntry = 'spl_ce_RecursiveIterator';
                break;

            case 'recursiveiteratoriterator':
                $classEntry = 'spl_ce_RecursiveIteratorIterator';
                break;

            case 'recursivetreeiterator':
                $classEntry = 'spl_ce_RecursiveTreeIterator';
                break;

            case 'filteriterator':
                $classEntry = 'spl_ce_FilterIterator';
                break;

            case 'recursivefilteriterator':
                $classEntry = 'spl_ce_RecursiveFilterIterator';
                break;

            case 'parentiterator':
                $classEntry = 'spl_ce_ParentIterator';
                break;

            case 'seekableiterator':
                $classEntry = 'spl_ce_SeekableIterator';
                break;

            case 'limititerator':
                $classEntry = 'spl_ce_LimitIterator';
                break;

            case 'cachingiterator':
                $classEntry = 'spl_ce_CachingIterator';
                break;

            case 'recursivecachingiterator':
                $classEntry = 'spl_ce_RecursiveCachingIterator';
                break;

            case 'outeriterator':
                $classEntry = 'spl_ce_OuterIterator';
                break;

            case 'iteratoriterator':
                $classEntry = 'spl_ce_IteratorIterator';
                break;

            case 'norewinditerator':
                $classEntry = 'spl_ce_NoRewindIterator';
                break;

            case 'infiniteiterator':
                $classEntry = 'spl_ce_InfiniteIterator';
                break;

            case 'emptyiterator':
                $classEntry = 'spl_ce_EmptyIterator';
                break;

            case 'appenditerator':
                $classEntry = 'spl_ce_AppendIterator';
                break;

            case 'regexiterator':
                $classEntry = 'spl_ce_RegexIterator';
                break;

            case 'recursiveregexiterator':
                $classEntry = 'spl_ce_RecursiveRegexIterator';
                break;

            case 'directoryiterator':
                $compilationContext->headersManager->add('ext/spl/spl_directory');
                $classEntry = 'spl_ce_DirectoryIterator';
                break;

            case 'filesystemiterator':
                $compilationContext->headersManager->add('ext/spl/spl_directory');
                $classEntry = 'spl_ce_FilesystemIterator';
                break;

            case 'recursivedirectoryiterator':
                $compilationContext->headersManager->add('ext/spl/spl_directory');
                $classEntry = 'spl_ce_RecursiveDirectoryIterator';
                break;

            case 'globiterator':
                $compilationContext->headersManager->add('ext/spl/spl_directory');
                $classEntry = 'spl_ce_GlobIterator';
                break;

            case 'splfileobject':
                $compilationContext->headersManager->add('ext/spl/spl_directory');
                $classEntry = 'spl_ce_SplFileObject';
                break;

            case 'spltempfileobject':
                $compilationContext->headersManager->add('ext/spl/spl_directory');
                $classEntry = 'spl_ce_SplTempFileObject';
                break;

            case 'countable':
                $classEntry = 'spl_ce_Countable';
                break;

            case 'callbackfilteriterator':
                $classEntry = 'spl_ce_CallbackFilterIterator';
                break;

            case 'recursivecallbackfilteriterator':
                $classEntry = 'spl_ce_RecursiveCallbackFilterIterator';
                break;

            case 'arrayobject':
                $compilationContext->headersManager->add('ext/spl/spl_array');
                $classEntry = 'spl_ce_ArrayObject';
                break;

            case 'splfixedarray':
                $compilationContext->headersManager->add('ext/spl/spl_fixedarray');
                $classEntry = 'spl_ce_SplFixedArray';
                break;

            case 'splpriorityqueue':
                $compilationContext->headersManager->add('ext/spl/spl_heap');
                $classEntry = 'spl_ce_SplPriorityQueue';
                break;

            case 'splfileinfo':
                $compilationContext->headersManager->add('ext/spl/spl_directory');
                $classEntry = 'spl_ce_SplFileInfo';
                break;

            case 'splheap':
                $compilationContext->headersManager->add('ext/spl/spl_heap');
                $classEntry = 'spl_ce_SplHeap';
                break;

            case 'splminheap':
                $compilationContext->headersManager->add('ext/spl/spl_heap');
                $classEntry = 'spl_ce_SplMinHeap';
                break;

            case 'splmaxheap':
                $compilationContext->headersManager->add('ext/spl/spl_heap');
                $classEntry = 'spl_ce_SplMaxHeap';
                break;

            case 'splstack':
                $compilationContext->headersManager->add('ext/spl/spl_dllist');
                $classEntry = 'spl_ce_SplStack';
                break;

            case 'splqueue':
                $compilationContext->headersManager->add('ext/spl/spl_dllist');
                $classEntry = 'spl_ce_SplQueue';
                break;

            case 'spldoublylinkedlist':
                $compilationContext->headersManager->add('ext/spl/spl_dllist');
                $classEntry = 'spl_ce_SplDoublyLinkedList';
                break;

            case 'stdclass':
                $classEntry = 'zend_standard_class_def';
                break;

            case 'closure':
                $compilationContext->headersManager->add('Zend/zend_closures');
                $classEntry = 'zend_ce_closure';
                break;

            case 'pdo':
                $compilationContext->headersManager->add('ext/pdo/php_pdo_driver');
                $classEntry = 'php_pdo_get_dbh_ce()';
                break;

            case 'pdostatement':
                $compilationContext->headersManager->add('kernel/main');
                $classEntry = 'zephir_get_internal_ce(SS("pdostatement") TSRMLS_CC)';
                break;

            case 'pdoexception':
                $compilationContext->headersManager->add('ext/pdo/php_pdo_driver');
                $classEntry = 'php_pdo_get_exception()';
                break;

            // Reflection
            /*case 'reflector':
                $compilationContext->headersManager->add('ext/reflection/php_reflection');
                $classEntry = 'reflector_ptr';
                break;
            case 'reflectionexception':
                $compilationContext->headersManager->add('ext/reflection/php_reflection');
                $classEntry = 'reflection_exception_ptr';
                break;
            case 'reflection':
                $compilationContext->headersManager->add('ext/reflection/php_reflection');
                $classEntry = 'reflection_ptr';
                break;
            case 'reflectionfunctionabstract':
                $compilationContext->headersManager->add('ext/reflection/php_reflection');
                $classEntry = 'reflection_function_abstract_ptr';
                break;
            case 'reflectionfunction':
                $compilationContext->headersManager->add('ext/reflection/php_reflection');
                $classEntry = 'reflection_function_ptr';
                break;
            case 'reflectionparameter':
                $compilationContext->headersManager->add('ext/reflection/php_reflection');
                $classEntry = 'reflection_parameter_ptr';
                break;
            case 'reflectionclass':
                $compilationContext->headersManager->add('ext/reflection/php_reflection');
                $classEntry = 'reflection_class_ptr';
                break;
            case 'reflectionobject':
                $compilationContext->headersManager->add('ext/reflection/php_reflection');
                $classEntry = 'reflection_object_ptr';
                break;
            case 'reflectionmethod':
                $compilationContext->headersManager->add('ext/reflection/php_reflection');
                $classEntry = 'reflection_method_ptr';
                break;
            case 'reflectionproperty':
                $compilationContext->headersManager->add('ext/reflection/php_reflection');
                $classEntry = 'reflection_property_ptr';
                break;
            case 'reflectionextension':
                $compilationContext->headersManager->add('ext/reflection/php_reflection');
                $classEntry = 'reflection_extension_ptr';
                break;
            case 'reflectionzendextension':
                $compilationContext->headersManager->add('ext/reflection/php_reflection');
                $classEntry = 'reflection_zend_extension_ptr';
                break;*/

            default:
                if (!$check) {
                    throw new CompilerException('Unknown class entry for "' . $className . '"');
                } else {
                    $classEntry = 'zephir_get_internal_ce(SS("' . Utils::escapeClassName(strtolower($className)) . '") TSRMLS_CC)';
                }
        }

        return $classEntry;
    }

    /**
     * Check if the host OS is windows
     *
     * @return boolean
     */
    public static function isWindows()
    {
        return strncasecmp(PHP_OS, 'WIN', 3) == 0;
    }
}
