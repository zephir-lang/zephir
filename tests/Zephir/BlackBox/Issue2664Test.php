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

use function basename;
use function file_put_contents;
use function sprintf;

/**
 * PHP concatenates the string form of any scalar. Zephir's `.=` used to accept
 * only a subset of operand types, and the gap behaved differently per left-hand
 * side: a local variable raised a compile error, while an object or a static
 * property silently overwrote the value instead of appending to it.
 *
 * A `double` must not be rendered by the compiler: PHP converts a float with
 * `zend_double_to_str()`, which reads `EG(precision)` at run time, so the
 * operand is boxed into a zval and handed to the engine instead. These
 * assertions therefore look for the boxing helpers rather than for a formatted
 * literal.
 *
 * Inspecting the generated C rather than running it keeps the whole matrix
 * testable from one container.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2664
 */
final class Issue2664Test extends TestCase
{
    use RunsZephirCommands;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testStringLeftHandSideTakesDoubleAndBoolOperands(): void
    {
        $generated = $this->generate(
            'catstring',
            'appends.zep',
            <<<'ZEP'
                namespace Catstring;

                class Appends
                {
                    public function doubleVar() -> string
                    {
                        string retval = "n=";
                        double d = 1.5;
                        let retval .= d;
                        return retval;
                    }

                    public function doubleLiteral() -> string
                    {
                        string retval = "n=";
                        let retval .= 1.5;
                        return retval;
                    }

                    public function boolVar() -> string
                    {
                        string retval = "n=";
                        bool b = true;
                        let retval .= b;
                        return retval;
                    }

                    public function boolTrueLiteral() -> string
                    {
                        string retval = "n=";
                        let retval .= true;
                        return retval;
                    }

                    public function boolFalseLiteral() -> string
                    {
                        string retval = "n=";
                        let retval .= false;
                        return retval;
                    }

                    public function nullLiteral() -> string
                    {
                        string retval = "n=";
                        let retval .= null;
                        return retval;
                    }
                }
                ZEP,
        );

        $this->assertStringContainsString('zephir_concat_self_double(&retval, d);', $generated);
        $this->assertStringContainsString('zephir_concat_self_double(&retval, 1.5);', $generated);
        $this->assertStringContainsString('zephir_concat_self_bool(&retval, b);', $generated);
        $this->assertStringContainsString('zephir_concat_self_bool(&retval, 1);', $generated);
        $this->assertStringContainsString('zephir_concat_self_bool(&retval, 0);', $generated);
        $this->assertStringContainsString('zephir_concat_self_str(&retval, SL(""));', $generated);
    }

    public function testVarLeftHandSideTakesDoubleAndBoolOperands(): void
    {
        $generated = $this->generate(
            'catvar',
            'appends.zep',
            <<<'ZEP'
                namespace Catvar;

                class Appends
                {
                    public function doubleVar() -> string
                    {
                        var retval = "n=";
                        double d = 1.5;
                        let retval .= d;
                        return retval;
                    }

                    public function doubleLiteral() -> string
                    {
                        var retval = "n=";
                        let retval .= 1.5;
                        return retval;
                    }

                    public function boolVar() -> string
                    {
                        var retval = "n=";
                        bool b = true;
                        let retval .= b;
                        return retval;
                    }

                    public function boolTrueLiteral() -> string
                    {
                        var retval = "n=";
                        let retval .= true;
                        return retval;
                    }

                    public function boolFalseLiteral() -> string
                    {
                        var retval = "n=";
                        let retval .= false;
                        return retval;
                    }

                    public function nullLiteral() -> string
                    {
                        var retval = "n=";
                        let retval .= null;
                        return retval;
                    }
                }
                ZEP,
        );

        $this->assertStringContainsString('zephir_concat_self_double(&retval, d);', $generated);
        $this->assertStringContainsString('zephir_concat_self_double(&retval, 1.5);', $generated);
        $this->assertStringContainsString('zephir_concat_self_bool(&retval, b);', $generated);
        $this->assertStringContainsString('zephir_concat_self_bool(&retval, 1);', $generated);
        $this->assertStringContainsString('zephir_concat_self_bool(&retval, 0);', $generated);
        $this->assertStringContainsString('zephir_concat_self_str(&retval, SL(""));', $generated);
    }

    /**
     * PHP raises "Array to string conversion" and appends "Array". A local
     * left-hand side used to reject the operand outright, while an object
     * property accepted it, so the two disagreed.
     */
    public function testArrayOperandIsConcatenatedOnALocalLeftHandSide(): void
    {
        $generated = $this->generate(
            'catarray',
            'appends.zep',
            <<<'ZEP'
                namespace Catarray;

                class Appends
                {
                    public function stringWithArrayLiteral() -> string
                    {
                        string retval = "n=";
                        let retval .= [1, 2];
                        return retval;
                    }

                    public function stringWithArrayVar() -> string
                    {
                        string retval = "n=";
                        array a = [1, 2];
                        let retval .= a;
                        return retval;
                    }

                    public function varWithArrayLiteral() -> string
                    {
                        var retval = "n=";
                        let retval .= [1, 2];
                        return retval;
                    }

                    public function varWithArrayVar() -> string
                    {
                        var retval = "n=";
                        array a = [1, 2];
                        let retval .= a;
                        return retval;
                    }

                    public function varWithEmptyArrayLiteral() -> string
                    {
                        var retval = "n=";
                        let retval .= [];
                        return retval;
                    }
                }
                ZEP,
        );

        $this->assertMethodEmits($generated, 'Catarray_Appends', 'stringWithArrayLiteral', 'zephir_concat_self(&retval, &_0);');
        $this->assertMethodEmits($generated, 'Catarray_Appends', 'stringWithArrayVar', 'zephir_concat_self(&retval, &a);');
        $this->assertMethodEmits($generated, 'Catarray_Appends', 'varWithArrayLiteral', 'zephir_concat_self(&retval, &_0);');
        $this->assertMethodEmits($generated, 'Catarray_Appends', 'varWithArrayVar', 'zephir_concat_self(&retval, &a);');
        $this->assertMethodEmits($generated, 'Catarray_Appends', 'varWithEmptyArrayLiteral', 'zephir_concat_self(&retval, &_0);');
    }

    public function testObjectPropertyConcatAssignAppendsEveryOperandType(): void
    {
        $generated = $this->generate(
            'catprop',
            'appends.zep',
            <<<'ZEP'
                namespace Catprop;

                class Appends
                {
                    private store = "";

                    public function intLiteral() -> void { let this->store .= 5; }
                    public function doubleLiteral() -> void { let this->store .= 1.5; }
                    public function boolLiteral() -> void { let this->store .= true; }
                    public function stringLiteral() -> void { let this->store .= "x"; }
                    public function nullLiteral() -> void { let this->store .= null; }

                    public function intVar() -> void { int n = 5; let this->store .= n; }
                    public function doubleVar() -> void { double d = 1.5; let this->store .= d; }
                    public function boolVar() -> void { bool b = true; let this->store .= b; }
                    public function charVar() -> void { char c = 'A'; let this->store .= c; }
                    public function stringVar(string s) -> void { let this->store .= s; }
                }
                ZEP,
        );

        $this->assertMethodEmits($generated, 'Catprop_Appends', 'intLiteral', 'zephir_concat_self_long(&_0, 5);');
        $this->assertMethodEmits($generated, 'Catprop_Appends', 'doubleLiteral', 'zephir_concat_self_double(&_0, 1.5);');
        $this->assertMethodEmits($generated, 'Catprop_Appends', 'boolLiteral', 'zephir_concat_self_bool(&_0, 1);');
        $this->assertMethodEmits($generated, 'Catprop_Appends', 'stringLiteral', 'zephir_concat_self_str(&_0, SL("x"));');
        $this->assertMethodEmits($generated, 'Catprop_Appends', 'nullLiteral', 'zephir_concat_self_str(&_0, SL(""));');
        $this->assertMethodEmits($generated, 'Catprop_Appends', 'intVar', 'zephir_concat_self_long(&_0, n);');
        $this->assertMethodEmits($generated, 'Catprop_Appends', 'doubleVar', 'zephir_concat_self_double(&_0, d);');
        $this->assertMethodEmits($generated, 'Catprop_Appends', 'boolVar', 'zephir_concat_self_bool(&_0, b);');
        $this->assertMethodEmits($generated, 'Catprop_Appends', 'charVar', 'zephir_concat_self_char(&_0, c);');
        $this->assertMethodEmits($generated, 'Catprop_Appends', 'stringVar', 'zephir_concat_self(&_0, &s_zv);');
    }

    public function testStaticPropertyConcatAssignAppendsEveryOperandType(): void
    {
        $generated = $this->generate(
            'catstatic',
            'appends.zep',
            <<<'ZEP'
                namespace Catstatic;

                class Appends
                {
                    protected static store = "";

                    public static function intLiteral() -> void { let self::store .= 5; }
                    public static function doubleLiteral() -> void { let self::store .= 1.5; }
                    public static function boolLiteral() -> void { let self::store .= true; }
                    public static function stringLiteral() -> void { let self::store .= "x"; }
                    public static function stringVar(string s) -> void { let self::store .= s; }
                }
                ZEP,
        );

        $this->assertMethodEmits($generated, 'Catstatic_Appends', 'intLiteral', 'zephir_concat_self_long(');
        $this->assertMethodEmits($generated, 'Catstatic_Appends', 'doubleLiteral', 'zephir_concat_self_double(');
        $this->assertMethodEmits($generated, 'Catstatic_Appends', 'boolLiteral', 'zephir_concat_self_bool(');
        $this->assertMethodEmits($generated, 'Catstatic_Appends', 'stringLiteral', 'zephir_concat_self_str(');
        $this->assertMethodEmits($generated, 'Catstatic_Appends', 'stringVar', 'zephir_concat_self(');
    }

    public function testBinaryConcatTakesBoolAndCharOperands(): void
    {
        $generated = $this->generate(
            'catbinary',
            'appends.zep',
            <<<'ZEP'
                namespace Catbinary;

                class Appends
                {
                    public function boolLiteral() -> string
                    {
                        return "n=" . true;
                    }

                    public function boolVar() -> string
                    {
                        bool b = true;
                        return "n=" . b;
                    }

                    public function charVar() -> string
                    {
                        char c = 'A';
                        return "n=" . c;
                    }
                }
                ZEP,
        );

        $this->assertMethodEmits($generated, 'Catbinary_Appends', 'boolLiteral', 'ZVAL_BOOL(&_0, 1);');
        $this->assertMethodEmits($generated, 'Catbinary_Appends', 'boolVar', 'ZVAL_BOOL(&_0, b);');
        $this->assertMethodEmits($generated, 'Catbinary_Appends', 'charVar', 'ZVAL_STRINGL(&_0, (char *) &c, 1);');
    }

    /**
     * Slices one `PHP_METHOD(...)` body out of the generated C so an assertion
     * cannot accidentally be satisfied by a different method's emit.
     */
    private function assertMethodEmits(
        string $generated,
        string $class,
        string $method,
        string $expected
    ): void {
        $marker = sprintf('PHP_METHOD(%s, %s)', $class, $method);
        $start  = strpos($generated, $marker);
        $this->assertIsInt($start, $marker . ' is missing from the generated C.');

        $end  = strpos($generated, 'PHP_METHOD(', $start + strlen($marker));
        $body = substr($generated, $start, false === $end ? null : $end - $start);

        $this->assertStringContainsString($expected, $body, $marker);
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
        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);

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
