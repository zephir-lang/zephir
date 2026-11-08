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

use function addcslashes;
use function bin2hex;
use function dirname;
use function file_get_contents;
use function file_put_contents;
use function is_dir;
use function json_encode;
use function mkdir;
use function random_bytes;
use function sys_get_temp_dir;

use const PHP_EOL;

/**
 * `__CLASS__`, `__NAMESPACE__`, `__METHOD__`, `__FUNCTION__` and `__TRAIT__`
 * fold to PHP's values: `__METHOD__` is `Class::method` with the declaring
 * class or trait, a function is named with its namespace, and a declaration
 * outside any function gets `""`.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2825
 */
final class Issue2825MagicConstantTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-issue2825m-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        mkdir($this->projectDir . '/stub', 0777, true);
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'stub', 'name' => 'stub'])
        );
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testMethodIsClassAndDeclaredName(): void
    {
        $generated = $this->generateOk('names.zep', <<<'ZEP'
            namespace Stub;

            class Names
            {
                public function runIt() -> string { return __METHOD__; }
                public static function staticRun() -> string { return __METHOD__; }
                public function func() -> string { return __FUNCTION__; }
            }
            ZEP);

        $this->assertStringContainsString('RETURN_STRING("Stub\\\\Names::runIt");', $generated);
        $this->assertStringContainsString('RETURN_STRING("Stub\\\\Names::staticRun");', $generated);
        $this->assertStringContainsString('RETURN_STRING("func");', $generated);
    }

    public function testNameInLowercaseNamespaceSegmentIsEscaped(): void
    {
        $generated = $this->generateOk('nested/thing.zep', <<<'ZEP'
            namespace Stub\nested;

            class Thing
            {
                public static function cls() -> string { return __CLASS__; }
                public static function ns() -> string { return __NAMESPACE__; }
                public static function method() -> string { return __METHOD__; }
            }
            ZEP);

        $this->assertStringContainsString('RETURN_STRING("Stub\\\\nested\\\\Thing");', $generated);
        $this->assertStringContainsString('RETURN_STRING("Stub\\\\nested");', $generated);
        $this->assertStringContainsString('RETURN_STRING("Stub\\\\nested\\\\Thing::method");', $generated);
    }

    public function testFunctionIsNamedWithItsNamespace(): void
    {
        $result = $this->generate([
            'holder.zep' => <<<'ZEP'
                namespace Stub;

                function magicFunction() -> string { return __FUNCTION__; }
                function magicMethod() -> string { return __METHOD__; }
                function magicClass() -> string { return __CLASS__; }

                class Holder
                {
                }
                ZEP,
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $generated = (string) file_get_contents($this->projectDir . '/ext/stub/holder.zep.c');
        $this->assertStringContainsString('RETURN_STRING("Stub\\\\magicFunction");', $generated);
        $this->assertStringContainsString('RETURN_STRING("Stub\\\\magicMethod");', $generated);
        $this->assertStringContainsString('RETURN_STRING("");', $generated);
    }

    public function testTraitMethodNamesTheTrait(): void
    {
        $result = $this->generate([
            'tr.zep' => <<<'ZEP'
                namespace Stub;

                trait Tr
                {
                    public function method() -> string { return __METHOD__; }
                    public function cls() -> string { return __CLASS__; }
                    public function traitName() -> string { return __TRAIT__; }
                    public function func() -> string { return __FUNCTION__; }
                }
                ZEP,
            'user.zep' => <<<'ZEP'
                namespace Stub;

                class User
                {
                    use Tr;

                    public function own() -> string { return __TRAIT__; }
                }
                ZEP,
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $user = (string) file_get_contents($this->projectDir . '/ext/stub/user.zep.c');
        $this->assertStringContainsString('RETURN_STRING("Stub\\\\Tr::method");', $user);
        $this->assertStringContainsString('RETURN_STRING("Stub\\\\User");', $user);
        $this->assertStringContainsString('RETURN_STRING("Stub\\\\Tr");', $user);
        $this->assertStringContainsString('RETURN_STRING("func");', $user);
        $this->assertStringContainsString('RETURN_STRING("");', $user);
    }

    public function testDeclarationsOutsideAFunction(): void
    {
        $generated = $this->generateOk('names.zep', <<<'ZEP'
            namespace Stub;

            class Names
            {
                const CLS = __CLASS__;
                const METHOD = __METHOD__;
                const FUNC = __FUNCTION__;
                const EOL = PHP_EOL;
            }
            ZEP);

        $this->assertStringContainsString('SL("CLS"), "Stub\\\\Names")', $generated);
        $this->assertStringContainsString('SL("METHOD"), "")', $generated);
        $this->assertStringContainsString('SL("FUNC"), "")', $generated);
        // PHP_EOL folds to the value of the PHP running Zephir: "\r\n" on Windows.
        $this->assertStringContainsString('SL("EOL"), "' . addcslashes(PHP_EOL, "\r\n") . '")', $generated);
    }

    /**
     * @param array<string, string> $files
     *
     * @return array{exitCode: int, stdout: string, stderr: string}
     */
    private function generate(array $files): array
    {
        foreach ($files as $name => $source) {
            $path = $this->projectDir . '/stub/' . $name;
            if (!is_dir(dirname($path))) {
                mkdir(dirname($path), 0777, true);
            }

            file_put_contents($path, $source . "\n");
        }

        return $this->runZephir('generate --no-ansi', $this->projectDir);
    }

    private function generateOk(string $fileName, string $source): string
    {
        $result = $this->generate([$fileName => $source]);
        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);

        return (string) file_get_contents($this->projectDir . '/ext/stub/' . $fileName . '.c');
    }
}
