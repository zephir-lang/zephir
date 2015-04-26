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

use Zephir\Config;
use Zephir\Logger;

/**
 * CompilerFileAnonymous
 *
 * This class represents an anonymous file created to dump
 * the code produced by an internal closure
 */
class CompilerFileAnonymous
{
    /**
     * @var string
     */
    protected $namespace;

    protected $compiledFile;

    protected $external = false;

    /**
     * @var ClassDefinition
     */
    protected $classDefinition;

    protected $headerCBlocks;

    /**
     * @var Config
     */
    protected $config = null;

    /**
     * @var Logger
     */
    protected $logger = null;

    /**
     * CompilerFileAnonymous constructor
     *
     * @param ClassDefinition $classDefinition
     * @param Config $config
     * @param Logger $logger
     */
    public function __construct(ClassDefinition $classDefinition, Config $config, Logger $logger)
    {
        $this->classDefinition = $classDefinition;
        $this->config = $config;
        $this->logger = $logger;
        $this->headerCBlocks = array();
    }

    /**
     * Returns the class definition related to the compiled file
     *
     * @return ClassDefinition
     */
    public function getClassDefinition()
    {
        return $this->classDefinition;
    }

    /**
     * Sets if the class belongs to an external dependency or not
     *
     * @param boolean $external
     */
    public function setIsExternal($external)
    {
        $this->external = (bool) $external;
    }

    /**
     * Checks if the class file belongs to an external dependency or not
     *
     * @return bool
     */
    public function isExternal()
    {
        return $this->external;
    }

    /**
     * Compiles the class/interface contained in the file
     *
     * @param ClassDefinition $classDefinition
     * @param CompilationContext $compilationContext
     */
    public function compileClass(ClassDefinition $classDefinition, CompilationContext $compilationContext)
    {
        $classDefinition = $this->classDefinition;

        /**
         * Do the compilation
         */
        $classDefinition->compile($compilationContext);

        $separators = str_repeat('../', count(explode('\\', $classDefinition->getCompleteName())) - 1);

        $code  = '' . PHP_EOL;
        $code .= '#ifdef HAVE_CONFIG_H' . PHP_EOL;
        $code .= '#include "' . $separators . 'ext_config.h"' . PHP_EOL;
        $code .= '#endif' . PHP_EOL;
        $code .= '' . PHP_EOL;

        $code .= '#include <php.h>' . PHP_EOL;
        $code .= '#include "' . $separators . 'php_ext.h"' . PHP_EOL;
        $code .= '#include "' . $separators . 'ext.h"' . PHP_EOL;
        $code .= '' . PHP_EOL;

        $code .= '#include <Zend/zend_operators.h>' . PHP_EOL;
        $code .= '#include <Zend/zend_exceptions.h>' . PHP_EOL;
        $code .= '#include <Zend/zend_interfaces.h>' . PHP_EOL;
        $code .= '' . PHP_EOL;

        $code .= '#include "kernel/main.h"' . PHP_EOL;

        if ($classDefinition->getType() == 'class') {
            foreach ($compilationContext->headersManager->get() as $header => $one) {
                $code .= '#include "' . $header . '.h"' . PHP_EOL;
            }
        }

        if (count($this->headerCBlocks) > 0) {
            $code .= implode($this->headerCBlocks, PHP_EOL) . PHP_EOL;
        }

        /**
         * Prepend the required files to the header
         */
        $compilationContext->codePrinter->preOutput($code);
    }

    /**
     * Compiles the file
     *
     * @param Compiler $compiler
     * @param StringsManager $stringsManager
     */
    public function compile(Compiler $compiler, StringsManager $stringsManager)
    {
        /**
         * Compilation context stores common objects required by compilation entities
         */
        $compilationContext = new CompilationContext;

        /**
         * Set global compiler in the compilation context
         */
        $compilationContext->compiler = $compiler;

        /**
         * Set global config in the compilation context
         */
        $compilationContext->config = $this->config;

        /**
         * Set global logger in the compilation context
         */
        $compilationContext->logger = $this->logger;

        /**
         * Set global strings manager
         */
        $compilationContext->stringsManager = $stringsManager;

        /**
         * Headers manager
         */
        $headersManager = new HeadersManager();
        $compilationContext->headersManager = $headersManager;

        /**
         * Main code-printer for the file
         */
        $codePrinter = new CodePrinter();
        $compilationContext->codePrinter = $codePrinter;

        /**
         * Alias manager
         */
        $compilationContext->aliasManager = new AliasManager();

        $codePrinter->outputBlankLine();

        $classDefinition = $this->classDefinition;

        $this->compileClass($classDefinition, $compilationContext);

        $completeName = $classDefinition->getCompleteName();

        $path = str_replace('\\', DIRECTORY_SEPARATOR, strtolower($completeName));

        $filePath       = 'ext/' . $path . '.zep.c';
        $filePathHeader = 'ext/' . $path . '.zep.h';

        if (strpos($path, DIRECTORY_SEPARATOR)) {
            $dirname = dirname($filePath);
            if (!is_dir($dirname)) {
                mkdir($dirname, 0755, true);
            }
        }

        if ($codePrinter) {
            /**
             * If the file does not exists we create it for the first time
             */
            if (!file_exists($filePath)) {
                file_put_contents($filePath, $codePrinter->getOutput());
                if ($compilationContext->headerPrinter) {
                    file_put_contents($filePathHeader, $compilationContext->headerPrinter->getOutput());
                }
            } else {
                /**
                 * Use md5 hash to avoid rewrite the file again and again when it hasn't changed
                 * thus avoiding unnecessary recompilations
                 */
                $output = $codePrinter->getOutput();
                $hash = hash_file('md5', $filePath);
                if (md5($output) != $hash) {
                    file_put_contents($filePath, $output);
                }

                if ($compilationContext->headerPrinter) {
                    $output = $compilationContext->headerPrinter->getOutput();
                    $hash = hash_file('md5', $filePathHeader);
                    if (md5($output) != $hash) {
                        file_put_contents($filePathHeader, $output);
                    }
                }
            }
        }

        /**
         * Add to file compiled
         */
        $this->compiledFile = $path . '.c';
    }

    /**
     * Returns the path to the compiled file
     *
     * @return string
     */
    public function getCompiledFile()
    {
        return $this->compiledFile;
    }
}
