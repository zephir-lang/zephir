<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use Psr\Log\LoggerAwareTrait;
use Psr\Log\NullLogger;
use Zephir\Compiler\FileInterface;

/**
 * CompilerFileAnonymous.
 *
 * This class represents an anonymous file created to dump
 * the code produced by an internal closure
 */
final class CompilerFileAnonymous implements FileInterface
{
    use LoggerAwareTrait;

    /** @var CompilationContext */
    protected $context;

    /** @var string */
    protected $namespace;

    /** @var string */
    protected $compiledFile;

    /** @var bool */
    protected $external = false;

    /** @var ClassDefinition */
    protected $classDefinition;

    protected $headerCBlocks = [];

    /** @var Config */
    protected $config;

    /**
     * CompilerFileAnonymous constructor.
     *
     * @param ClassDefinition $classDefinition
     * @param Config          $config
     */
    public function __construct(ClassDefinition $classDefinition, Config $config, CompilationContext $context = null)
    {
        $this->classDefinition = $classDefinition;
        $this->config = $config;
        $this->context = $context;
        $this->logger = new NullLogger();
    }

    /**
     * {@inheritdoc}
     *
     * @return ClassDefinition
     */
    public function getClassDefinition()
    {
        return $this->classDefinition;
    }

    /**
     * Sets if the class belongs to an external dependency or not.
     *
     * @param bool $external
     */
    public function setIsExternal($external)
    {
        $this->external = (bool) $external;
    }

    /**
     * {@inheritdoc}
     *
     * @return bool
     */
    public function isExternal()
    {
        return $this->external;
    }

    /**
     * Compiles the class/interface contained in the file.
     *
     * @param CompilationContext $compilationContext
     *
     * @throws Exception
     */
    private function compileClass(CompilationContext $compilationContext)
    {
        $classDefinition = $this->classDefinition;

        /*
         * Do the compilation
         */
        $classDefinition->compile($compilationContext);

        $separators = str_repeat('../', \count(explode('\\', $classDefinition->getCompleteName())) - 1);

        $code = ''.PHP_EOL;
        $code .= '#ifdef HAVE_CONFIG_H'.PHP_EOL;
        $code .= '#include "'.$separators.'ext_config.h"'.PHP_EOL;
        $code .= '#endif'.PHP_EOL;
        $code .= ''.PHP_EOL;

        $code .= '#include <php.h>'.PHP_EOL;
        $code .= '#include "'.$separators.'php_ext.h"'.PHP_EOL;
        $code .= '#include "'.$separators.'ext.h"'.PHP_EOL;
        $code .= ''.PHP_EOL;

        $code .= '#include <Zend/zend_operators.h>'.PHP_EOL;
        $code .= '#include <Zend/zend_exceptions.h>'.PHP_EOL;
        $code .= '#include <Zend/zend_interfaces.h>'.PHP_EOL;
        $code .= ''.PHP_EOL;

        $code .= '#include "kernel/main.h"'.PHP_EOL;

        if ('class' == $classDefinition->getType()) {
            foreach ($compilationContext->headersManager->get() as $header => $one) {
                $code .= '#include "'.$header.'.h"'.PHP_EOL;
            }
        }

        if (\count($this->headerCBlocks) > 0) {
            $code .= implode(PHP_EOL, $this->headerCBlocks).PHP_EOL;
        }

        /*
         * Prepend the required files to the header
         */
        $compilationContext->codePrinter->preOutput($code);
    }

    /**
     * Compiles the file.
     *
     * @param Compiler       $compiler
     * @param StringsManager $stringsManager
     *
     * @throws Exception
     */
    public function compile(Compiler $compiler, StringsManager $stringsManager)
    {
        /**
         * Compilation context stores common objects required by compilation entities.
         */
        $compilationContext = new CompilationContext();

        if ($this->context) {
            $compilationContext->aliasManager = $this->context->aliasManager;
        } else {
            $compilationContext->aliasManager = new AliasManager();
        }

        /*
         * Set global compiler in the compilation context
         */
        $compilationContext->compiler = $compiler;

        /*
         * Set global config in the compilation context
         */
        $compilationContext->config = $this->config;

        /*
         * Set global logger in the compilation context
         */
        $compilationContext->logger = $this->logger;

        /*
         * Set global strings manager
         */
        $compilationContext->stringsManager = $stringsManager;

        $compilationContext->backend = $compiler->backend;

        /**
         * Headers manager.
         */
        $headersManager = new HeadersManager();
        $compilationContext->headersManager = $headersManager;

        /**
         * Main code-printer for the file.
         */
        $codePrinter = new CodePrinter();
        $compilationContext->codePrinter = $codePrinter;

        $codePrinter->outputBlankLine();

        $this->compileClass($compilationContext);

        $completeName = $this->classDefinition->getCompleteName();

        $path = str_replace('\\', \DIRECTORY_SEPARATOR, strtolower($completeName));

        $filePath = 'ext/'.$path.'.zep.c';
        $filePathHeader = 'ext/'.$path.'.zep.h';

        if (strpos($path, \DIRECTORY_SEPARATOR)) {
            $dirname = \dirname($filePath);
            if (!is_dir($dirname)) {
                mkdir($dirname, 0755, true);
            }
        }

        if ($codePrinter) {
            /*
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
                 * thus avoiding unnecessary recompilations.
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

        /*
         * Add to file compiled
         */
        $this->compiledFile = $path.'.c';
    }

    /**
     * Returns the path to the compiled file.
     *
     * @return string
     */
    public function getCompiledFile()
    {
        return $this->compiledFile;
    }

    /**
     * {@inheritdoc}
     *
     * Only implemented to satisfy the Zephir\Compiler\FileInterface interface.
     *
     * @param Compiler $compiler
     */
    public function preCompile(Compiler $compiler)
    {
        // nothing to do
    }
}
