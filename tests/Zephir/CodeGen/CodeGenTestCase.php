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

namespace Zephir\Test\CodeGen;

use PHPUnit\Framework\TestCase;
use Psr\Log\NullLogger;
use Zephir\AliasManager;
use Zephir\Backend\Backend;
use Zephir\Branch;
use Zephir\BranchManager;
use Zephir\Code\Printer;
use Zephir\CompilationContext;
use Zephir\Config;
use Zephir\SymbolTable;

use function preg_replace;
use function strpos;
use function trim;

/**
 * Base test case for code generation tests.
 *
 * Provides helpers to test C code generation from Zephir AST.
 * Enables unit testing of individual code generation methods and
 * supports refactoring toward backend abstraction.
 */
abstract class CodeGenTestCase extends TestCase
{
    protected CompilationContext $context;
    protected Printer $codePrinter;
    protected Backend $backend;
    protected Config $config;

    protected function setUp(): void
    {
        parent::setUp();

        // Create minimal config
        $this->config = new Config();

        // Create backend with empty paths (tests don't need templates)
        $this->backend = new Backend($this->config, '', '');

        // Create code printer to capture output
        $this->codePrinter = new Printer();

        // Setup compilation context with all required components
        $this->context                  = new CompilationContext();
        $this->context->config          = $this->config;
        $this->context->backend         = $this->backend;
        $this->context->codePrinter     = $this->codePrinter;
        $this->context->aliasManager    = new AliasManager();
        $this->context->branchManager   = new BranchManager();
        $this->context->logger          = new NullLogger();

        // Initialize branch manager with root branch (required for variable tracking)
        $rootBranch = new Branch();
        $rootBranch->setType(Branch::TYPE_ROOT);
        $this->context->branchManager->addBranch($rootBranch);

        $this->context->symbolTable     = new SymbolTable($this->context);

        $this->context->symbolTable     = new SymbolTable($this->context);
    }

    /**
     * Get generated C code output.
     */
    protected function getOutput(): string
    {
        return $this->codePrinter->getOutput();
    }

    /**
     * Clear the code printer output.
     */
    protected function clearOutput(): void
    {
        $this->codePrinter->clear();
    }

    /**
     * Assert that output contains a specific C code pattern.
     */
    protected function assertOutputContains(string $needle, string $message = ''): void
    {
        $output = $this->getOutput();
        $this->assertStringContainsString($needle, $output, $message ?: "Output should contain: {$needle}");
    }

    /**
     * Assert that output does not contain a specific pattern.
     */
    protected function assertOutputNotContains(string $needle, string $message = ''): void
    {
        $output = $this->getOutput();
        $this->assertStringNotContainsString($needle, $output, $message ?: "Output should not contain: {$needle}");
    }

    /**
     * Assert that output matches a regex pattern.
     */
    protected function assertOutputMatches(string $pattern, string $message = ''): void
    {
        $output = $this->getOutput();
        $this->assertMatchesRegularExpression($pattern, $output, $message);
    }

    /**
     * Assert that output contains lines in specific order.
     * Useful for verifying code structure.
     */
    protected function assertOutputContainsInOrder(array $needles, string $message = ''): void
    {
        $output  = $this->getOutput();
        $lastPos = -1;

        foreach ($needles as $needle) {
            $pos = strpos($output, $needle);
            $this->assertNotFalse($pos, "Could not find: {$needle}");
            $this->assertGreaterThan(
                $lastPos,
                $pos,
                "Order violation for: {$needle}. Expected after position {$lastPos}, found at {$pos}. " . $message
            );
            $lastPos = $pos;
        }
    }

    /**
     * Normalize C code for comparison (removes extra whitespace, comments).
     * Useful for flexible assertions that ignore formatting differences.
     */
    protected function normalizeCode(string $code): string
    {
        // Remove C-style comments
        $code = preg_replace('#/\*.*?\*/#s', '', $code);
        // Remove C++ style comments
        $code = preg_replace('#//.*$#m', '', $code);
        // Normalize whitespace to single spaces
        $code = preg_replace('/\s+/', ' ', $code);
        // Trim
        $code = trim($code);

        return $code;
    }

    /**
     * Assert exact C code match with normalization.
     * Use this for strict verification while allowing formatting differences.
     */
    protected function assertCodeEquals(string $expected, string $actual, string $message = ''): void
    {
        $this->assertEquals(
            $this->normalizeCode($expected),
            $this->normalizeCode($actual),
            $message
        );
    }

    /**
     * Assert that generated output equals expected code with normalization.
     */
    protected function assertOutputEquals(string $expected, string $message = ''): void
    {
        $this->assertCodeEquals($expected, $this->getOutput(), $message);
    }

    /**
     * Count occurrences of a string in output.
     */
    protected function countInOutput(string $needle): int
    {
        return substr_count($this->getOutput(), $needle);
    }

    /**
     * Create a minimal mock method object for testing.
     * Required by SymbolTable for variable ID generation.
     */
    private function createMockMethod(): object
    {
        return new class {
            private static int $uniqueId = 1;

            public function getUniqueId(): int
            {
                return self::$uniqueId++;
            }
        };
    }
}
