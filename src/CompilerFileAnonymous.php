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

namespace Zephir;

use Psr\Log\LoggerAwareTrait;
use Psr\Log\NullLogger;
use ReflectionException;
use Zephir\Class\Definition\Definition;
use Zephir\Code\Printer;
use Zephir\Compiler\FileInterface;
use Zephir\Traits\CompilerTrait;

use function file_exists;
use function file_put_contents;
use function hash_file;
use function md5;

use const PHP_EOL;

/**
 * This class represents an anonymous file created to dump
 * the code produced by an internal closure
 */
final class CompilerFileAnonymous implements FileInterface
{
    use CompilerTrait;
    use LoggerAwareTrait;

    protected ?string             $compiledFile  = null;
    protected bool                $external      = false;
    protected BlockManager        $headerCBlocks;
    protected ?string             $namespace     = null;

    /**
     * CompilerFileAnonymous constructor.
     *
     * @param Definition              $classDefinition
     * @param Config                  $config
     * @param CompilationContext|null $context
     */
    public function __construct(
        protected Definition $classDefinition,
        protected Config $config,
        protected ?CompilationContext $context = null
    ) {
        $this->logger = new NullLogger();

        $this->headerCBlocks = new BlockManager();
    }

    /**
     * Compiles the file.
     *
     * @throws Exception|ReflectionException
     */
    public function compile(Compiler $compiler, StringsManager $stringsManager): void
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

        $compilationContext->compiler       = $compiler;
        $compilationContext->config         = $this->config;
        $compilationContext->logger         = $this->logger;
        $compilationContext->stringsManager = $stringsManager;
        $compilationContext->backend        = $compiler->backend;
        $compilationContext->headersManager = new HeadersManager();

        /**
         * C-Block manager.
         */
        $compilationContext->blockManager   = $this->headerCBlocks;

        /**
         * Main code-printer for the file.
         */
        $codePrinter                     = new Printer();
        $compilationContext->codePrinter = $codePrinter;

        $codePrinter->outputBlankLine();

        $this->compileClass($compilationContext);

        $completeName = $this->classDefinition->getCompleteName();

        [$path, $filePath, $filePathHeader] = $this->calculatePaths($completeName);

        /**
         * If the file does not exist we create it for the first time
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
            $hash   = hash_file('md5', $filePath);
            if (md5($output) !== $hash) {
                file_put_contents($filePath, $output);
            }

            if ($compilationContext->headerPrinter) {
                $output = $compilationContext->headerPrinter->getOutput();
                $hash   = hash_file('md5', $filePathHeader);
                if (md5($output) !== $hash) {
                    file_put_contents($filePathHeader, $output);
                }
            }
        }

        /**
         * Add to file compiled
         */
        $this->compiledFile = $path . '.c';
    }

    public function getClassDefinition(): Definition
    {
        return $this->classDefinition;
    }

    /**
     * Returns the path to the compiled file.
     */
    public function getCompiledFile(): string
    {
        return $this->compiledFile;
    }

    public function isExternal(): bool
    {
        return $this->external;
    }

    /**
     * Only implemented to satisfy the Zephir\Compiler\FileInterface interface.
     */
    public function preCompile(Compiler $compiler): void
    {
        // nothing to do
    }

    /**
     * Sets if the class belongs to an external dependency or not.
     */
    public function setIsExternal($external): void
    {
        $this->external = (bool)$external;
    }

    /**
     * Compiles the class/interface contained in the file.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function compileClass(CompilationContext $compilationContext): void
    {
        /**
         * Do the compilation
         */
        $this->classDefinition->compile($compilationContext);

        $code = $this->generateCodeHeadersPre($this->classDefinition);

        $code .= '#include <Zend/zend_operators.h>' . PHP_EOL;
        $code .= '#include <Zend/zend_exceptions.h>' . PHP_EOL;
        $code .= '#include <Zend/zend_interfaces.h>' . PHP_EOL;

        $this->generateClassHeadersPost($code, $this->classDefinition, $compilationContext);
    }
}
