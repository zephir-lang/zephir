<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Test\Backends\ZendEngine3;

use PHPUnit\Framework\TestCase;
use Zephir\Backend\Backend;
use Zephir\Branch;
use Zephir\BranchManager;
use Zephir\Code\Printer;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Config;
use Zephir\HeadersManager;
use Zephir\SymbolTable;
use Zephir\Variable\Variable;

final class BackendTest extends TestCase
{
    public function stringDataProvider(): array
    {
        return [
            'regexp1' => ['/(\w+)\s*=\s*(\[[^\]]*\]|\"[^\"]*\"|[^,)]*)\s*(?:,|$)/', '/(\\w+)\\s*=\\s*(\\[[^\\]]*\\]|\"[^\"]*\"|[^,)]*)\\s*(?:,|$)/'],
            'regexp2' => ['/@(\w+)(?:\s*(?:\(\s*)?(.*?)(?:\s*\))?)??\s*(?:\n|\*\/)/', '/@(\\w+)(?:\\s*(?:\\(\\s*)?(.*?)(?:\\s*\\))?)??\\s*(?:\n|\\*\\/)/'],
            'simple string' => ['zephir', 'zephir'],
            'string with slashed' => ['\\Zephir', '\Zephir'],
            'string with \n' => ['\\Zephir\n', '\Zephir\\n'],
            'string with \null' => ['\\null', '\null'],
        ];
    }

    /**
     * @dataProvider stringDataProvider
     *
     * @param string $testString
     * @param string $expectedString
     */
    public function testShouldEscapeStringWithRegexp(string $testString, string $expectedString): void
    {
        $backend = new Backend(new Config(), '', '');
        $variable = new Variable('variable', 'name');
        $context = new CompilationContext();

        $actual = $backend->assignString($variable, $testString, $context, false);
        $expected = 'ZVAL_STRING(&name, "'.$expectedString.'");';

        $this->assertSame($expected, $actual);
    }

    /**
     * Zephir's four integer types all describe a PHP `int`, which the engine
     * stores in a `zend_long` (`int64_t` on every 64-bit target). Mapping any
     * of them to a C `long` makes the width follow the data model: 64-bit on
     * LP64 (Linux, macOS), 32-bit on LLP64 (Windows x64).
     *
     * @see https://github.com/zephir-lang/zephir/issues/2666
     */
    public function integerTypeDefinitionProvider(): array
    {
        return [
            'int'   => ['int', 'zend_long'],
            'uint'  => ['uint', 'zend_ulong'],
            'long'  => ['long', 'zend_long'],
            'ulong' => ['ulong', 'zend_ulong'],
        ];
    }

    /**
     * @dataProvider integerTypeDefinitionProvider
     */
    public function testIntegerTypesMapToAFixedWidthCType(string $type, string $expected): void
    {
        $backend = new Backend(new Config(), '', '');

        $this->assertSame([null, $expected], $backend->getTypeDefinition($type));
    }

    /**
     * `SL(s)` is `s, sizeof(s) - 1`; `SS(s)` is `s, sizeof(s)`. A key length
     * that counts the trailing NUL is a key no PHP array can hold, so the call
     * silently matches nothing. Only `SL()` is correct for a C string literal
     * used as an array key.
     *
     * Every offset shape is asserted, not just the string one, because the
     * defect was a whole duplicated branch drifting away from its twin rather
     * than one wrong character.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2708
     */
    public function arrayOffsetProvider(): array
    {
        return [
            'string literal' => [
                new CompiledExpression('string', 'key_a', []),
                'zephir_array_unset_string(&data, SL("key_a"), PH_SEPARATE);',
            ],
            'int literal' => [
                new CompiledExpression('int', '3', []),
                'zephir_array_unset_long(&data, 3, PH_SEPARATE);',
            ],
            'long literal' => [
                new CompiledExpression('long', '3', []),
                'zephir_array_unset_long(&data, 3, PH_SEPARATE);',
            ],
            'native int variable' => [
                new CompiledExpression('variable', 'idx', []),
                'zephir_array_unset_long(&data, idx, PH_SEPARATE);',
            ],
            'var variable' => [
                new CompiledExpression('variable', 'key', []),
                'zephir_array_unset(&data, &key, PH_SEPARATE);',
            ],
        ];
    }

    /**
     * @dataProvider arrayOffsetProvider
     */
    public function testArrayUnsetEmitsTheExactKeyLength(CompiledExpression $offset, string $expected): void
    {
        $backend = new Backend(new Config(), '', '');
        $context = $this->wiredContext();

        $backend->arrayUnset(new Variable('variable', 'data'), $offset, 'PH_SEPARATE', $context);

        $this->assertSame($expected . PHP_EOL, $context->codePrinter->getOutput());
    }

    /**
     * `arrayUnset2()` is the deprecated entry point. It used to hold a second
     * copy of the switch, reached for every offset shape but the string one,
     * which `arrayUnset()` handled before delegating: the branch nobody could
     * reach was exactly the branch that drifted. Asserting both entry points
     * emit the same C is what keeps a copy from growing back.
     *
     * @dataProvider arrayOffsetProvider
     */
    public function testArrayUnset2EmitsTheExactKeyLength(CompiledExpression $offset, string $expected): void
    {
        $backend = new Backend(new Config(), '', '');
        $context = $this->wiredContext();

        $backend->arrayUnset2(new Variable('variable', 'data'), $offset, 'PH_SEPARATE', $context);

        $this->assertSame($expected . PHP_EOL, $context->codePrinter->getOutput());
    }

    /**
     * `arrayIssetFetch()` / `arrayIssetFetch2()` carried the same split, and the
     * same stale copy behind it.
     *
     * A `Variable` instance rather than a `CompiledExpression` is what these two
     * dispatch on, so the provider hands over both shapes.
     */
    public function issetFetchOffsetProvider(): array
    {
        return [
            'string literal' => [
                new CompiledExpression('string', 'key_a', []),
                'zephir_array_isset_string_fetch(&found, &data, SL("key_a"), 1)',
            ],
            'int literal' => [
                new CompiledExpression('int', '3', []),
                'zephir_array_isset_long_fetch(&found, &data, 3, 1)',
            ],
            'native int variable' => [
                new Variable('int', 'idx'),
                'zephir_array_isset_long_fetch(&found, &data, idx, 1)',
            ],
            'var variable' => [
                new Variable('variable', 'key'),
                'zephir_array_isset_fetch(&found, &data, &key, 1)',
            ],
        ];
    }

    /**
     * @dataProvider issetFetchOffsetProvider
     */
    public function testArrayIssetFetchEmitsTheExactKeyLength(mixed $offset, string $expected): void
    {
        $backend = new Backend(new Config(), '', '');

        $this->assertSame(
            $expected,
            $backend->arrayIssetFetch(
                new Variable('variable', 'found'),
                new Variable('variable', 'data'),
                $offset,
                '1',
                [],
                $this->wiredContext()
            )->getCode()
        );
    }

    /**
     * @dataProvider issetFetchOffsetProvider
     */
    public function testArrayIssetFetch2EmitsTheExactKeyLength(mixed $offset, string $expected): void
    {
        $backend = new Backend(new Config(), '', '');

        $this->assertSame(
            $expected,
            $backend->arrayIssetFetch2(
                new Variable('variable', 'found'),
                new Variable('variable', 'data'),
                $offset,
                '1',
                [],
                $this->wiredContext()
            )->getCode()
        );
    }

    /**
     * `arrayIsset()` holds both copies itself: the second guarded block is
     * byte-equivalent to the first apart from the macro, and the first returns,
     * so the second can never run. This pins what survives the merge.
     */
    public function testArrayIssetEmitsTheExactKeyLength(): void
    {
        $backend = new Backend(new Config(), '', '');

        $this->assertSame(
            'zephir_array_isset_value_string(&data, SL("key_a"))',
            $backend->arrayIsset(
                new Variable('variable', 'data'),
                new CompiledExpression('string', 'key_a', []),
                []
            )->getCode()
        );
    }

    /**
     * A `CompilationContext` is a plain bag of nullable public properties, so
     * the collaborators the array-offset emitters touch are assigned by hand.
     *
     * The order follows Method::compile(): a fictitious external branch is
     * registered first, because `SymbolTable` reads the current branch in its
     * constructor and every later lookup walks up from it.
     */
    private function wiredContext(): CompilationContext
    {
        $branch = new Branch();
        $branch->setType(Branch::TYPE_EXTERNAL);

        $context                 = new CompilationContext();
        $context->config         = new Config();
        $context->codePrinter    = new Printer();
        $context->headersManager = new HeadersManager();
        $context->branchManager  = new BranchManager();
        $context->branchManager->addBranch($branch);
        $context->symbolTable    = new SymbolTable($context);

        foreach (['int' => 'idx', 'variable' => 'key'] as $type => $name) {
            $variable = new Variable($type, $name);
            $variable->setIsInitialized(true, $context);
            $context->symbolTable->addRawVariable($variable);
        }

        return $context;
    }
}
