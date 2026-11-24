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

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

/**
 * `Z_PARAM_BOOL()`, `Z_PARAM_LONG()` and `Z_PARAM_DOUBLE()` write the C local
 * of an optional parameter only when the argument is passed. The local used to
 * be declared without a value and given its default afterwards, in
 * `if (!x_param) { x = default; }`. gcc 11 -O2 merged that into
 * `x = (x_param == NULL) | x`, which reads the local nobody wrote: a
 * `zend_bool` that left the register as 0xf1 became a zval of type 0xf3 and
 * crashed Phalcon.
 *
 * PHP's own functions declare `bool x = true;` and let ZPP overwrite it, so
 * the local holds a defined value on every path. Which garbage a register
 * holds cannot be tested, so the generated C is.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2857
 */
final class Issue2857Test extends TestCase
{
    use RunsZephirCommands;

    private const SOURCE = <<<'ZEP'
        namespace Optdefault;

        class Params
        {
            const LIMIT = 10;
            const ON = true;
            const RATIO = 0.5;

            public function getMessages(var type = null, bool remove = true) -> array
            {
                return this->getSessionMessages(remove, type);
            }

            protected function getSessionMessages(bool remove, var type = null) -> array
            {
                return [remove, type];
            }

            public function boolFalse(bool flag = false) -> bool
            {
                return flag;
            }

            public function boolNull(bool flag = null) -> bool
            {
                return flag;
            }

            public function intValue(int number = 1337) -> int
            {
                return number;
            }

            public function intNull(int number = null) -> int
            {
                return number;
            }

            public function uintValue(uint number = 7) -> int
            {
                return number;
            }

            public function longNegative(long number = -5) -> int
            {
                return number;
            }

            public function ulongValue(ulong number = 9) -> int
            {
                return number;
            }

            public function doubleValue(double ratio = 13.37) -> double
            {
                return ratio;
            }

            public function doubleFromInt(double ratio = 2) -> double
            {
                return ratio;
            }

            public function doubleNull(double ratio = null) -> double
            {
                return ratio;
            }

            public function intConstant(int number = self::LIMIT) -> int
            {
                return number;
            }

            public function boolConstant(bool flag = self::ON) -> bool
            {
                return flag;
            }

            public function doubleConstant(double ratio = self::RATIO) -> double
            {
                return ratio;
            }

            public function strictBool(bool! flag = true) -> bool
            {
                return flag;
            }
        }
        ZEP;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    /**
     * @return array<string, array{string, string, string, string}>
     */
    public static function optionalScalarProvider(): array
    {
        return [
            'bool true, after a var'  => ['getMessages', 'zend_bool', 'remove', '1'],
            'bool false'              => ['boolFalse', 'zend_bool', 'flag', '0'],
            'bool null'               => ['boolNull', 'zend_bool', 'flag', '0'],
            'int'                     => ['intValue', 'zend_long', 'number', '1337'],
            'int null'                => ['intNull', 'zend_long', 'number', '0'],
            'uint'                    => ['uintValue', 'zend_ulong', 'number', '7'],
            'long negative'           => ['longNegative', 'zend_long', 'number', '-5'],
            'ulong'                   => ['ulongValue', 'zend_ulong', 'number', '9'],
            'double'                  => ['doubleValue', 'double', 'ratio', '13.37'],
            'double from int literal' => ['doubleFromInt', 'double', 'ratio', '(double) 2'],
            'double null'             => ['doubleNull', 'double', 'ratio', '0'],
            'int class constant'      => ['intConstant', 'zend_long', 'number', '10'],
            'bool class constant'     => ['boolConstant', 'zend_bool', 'flag', '1'],
            'double class constant'   => ['doubleConstant', 'double', 'ratio', '0.5'],
        ];
    }

    /**
     * @dataProvider optionalScalarProvider
     */
    public function testOptionalScalarIsDeclaredWithItsDefault(
        string $method,
        string $cType,
        string $name,
        string $default
    ): void {
        $body = $this->methodBody($method);

        $this->assertMatchesRegularExpression(
            '/^\t' . preg_quote($cType, '/') . ' [^;]*\b' . $name . ' = ' . preg_quote($default, '/') . '[,;]/m',
            $body,
        );
    }

    /**
     * @dataProvider optionalScalarProvider
     */
    public function testOptionalScalarDropsTheDefaultAssignmentBlock(
        string $method,
        string $cType,
        string $name,
        string $default
    ): void {
        $this->assertStringNotContainsString('if (!' . $name . '_param)', $this->methodBody($method));
    }

    public function testStrictOptionalScalarIsDeclaredWithItsDefault(): void
    {
        $this->assertMatchesRegularExpression('/^\tzend_bool [^;]*\bflag = 1[,;]/m', $this->methodBody('strictBool'));
    }

    public function testStrictOptionalScalarKeepsItsTypeCheck(): void
    {
        $this->assertStringContainsString(
            'Z_TYPE_P(flag_param) != IS_TRUE',
            $this->methodBody('strictBool'),
        );
    }

    /**
     * The body of one generated method, from its PHP_METHOD line to the next,
     * so an assertion cannot be satisfied by another method's code.
     */
    private function methodBody(string $method): string
    {
        $generated = $this->generate('optdefault', 'params.zep', self::SOURCE);
        $marker    = 'PHP_METHOD(Optdefault_Params, ' . $method . ')';
        $start     = strpos($generated, $marker);
        $this->assertIsInt($start, $marker . ' is missing from the generated C.');

        $end = strpos($generated, 'PHP_METHOD(', $start + strlen($marker));

        return substr($generated, $start, false === $end ? null : $end - $start);
    }

    private function generate(string $project, string $fileName, string $source): string
    {
        $projectDir = $this->outputDir() . '/' . $project;
        $this->cleanupPath($projectDir);

        $this->assertSame(
            0,
            $this->runZephir('init ' . $project, $this->outputDir())['exitCode'],
        );

        file_put_contents($projectDir . '/' . $project . '/' . $fileName, $source . "\n");

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr']);

        $generatedFile = sprintf(
            '%s/ext/%s/%s.c',
            $projectDir,
            $project,
            basename($fileName, '.zep') . '.zep',
        );
        $this->assertFileExists($generatedFile);

        return (string) file_get_contents($generatedFile);
    }
}
