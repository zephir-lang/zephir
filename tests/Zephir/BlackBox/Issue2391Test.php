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
 * Property shortcuts stay supported: a typed `return this->prop` checks the
 * value against the method's return type, a static property shortcut uses
 * static property access, and a PHPDoc `@var` type never fails the build.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2391
 */
final class Issue2391Test extends TestCase
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

    /**
     * @dataProvider verifiedGetterProvider
     */
    public function testTypedPropertyReturnIsVerified(string $declaration, string $method): void
    {
        $code = $this->generatedCode('Verified', <<<ZEP
            namespace Probe;

            class Verified
            {
                {$declaration}

                public function byHand() -> bool
                {
                    return this->flag;
                }
            }
            ZEP);

        $this->assertMatchesRegularExpression(
            '/PHP_METHOD\(Probe_Verified, ' . $method . '\)\s*\{[^}]*RETURN_(MM_)?MEMBER_VERIFIED\(/',
            $code
        );
    }

    public function verifiedGetterProvider(): array
    {
        return [
            'bool shortcut'      => ["/** @var bool */\n    protected flag { get };", 'getFlag'],
            'class shortcut'     => ["/** @var \\Probe\\Verified */\n    protected flag { get };", 'getFlag'],
            'nullable shortcut'  => ["/** @var Verified|null */\n    protected flag { get };", 'getFlag'],
            'scalar shortcut'    => ["/** @var string */\n    protected flag { get };", 'getFlag'],
            'hand-written bool'  => ['protected flag;', 'byHand'],
        ];
    }

    public function testUntypedPropertyReturnIsNotVerified(): void
    {
        $code = $this->generatedCode('Untyped', <<<'ZEP'
            namespace Probe;

            class Untyped
            {
                protected value { get };
            }
            ZEP);

        $this->assertStringContainsString('RETURN_MEMBER(getThis(), "value");', $code);
    }

    public function testStaticPropertyShortcutsUseStaticAccess(): void
    {
        $code = $this->generatedCode('StaticHolder', <<<'ZEP'
            namespace Probe;

            class StaticHolder
            {
                protected static value = 1 { get, set };
            }
            ZEP);

        $this->assertStringContainsString('zephir_read_static_property_ce(', $code);
        $this->assertStringContainsString('zephir_update_static_property_ce(', $code);
        $this->assertStringNotContainsString('getThis(), "value"', $code);
    }

    /**
     * @dataProvider docblockTypeProvider
     */
    public function testDocblockTypeCompiles(string $type, string $getterArgInfo): void
    {
        $header = $this->generatedHeader('Doc', <<<ZEP
            namespace Probe;

            class Doc
            {
                /** @var {$type} */
                protected value { get, set };
            }
            ZEP);

        $this->assertMatchesRegularExpression(
            '/' . $getterArgInfo . '\(arginfo_probe_doc_getvalue, 0, 0[^)]*\)/',
            $header
        );
    }

    public function docblockTypeProvider(): array
    {
        return [
            'float'          => ['float', 'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX'],
            'integer'        => ['integer', 'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX'],
            'boolean'        => ['boolean', 'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX'],
            '?int'           => ['?int', 'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX'],
            'string[]'       => ['string[]', 'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX'],
            'array<int>'     => ['array<int>', 'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX'],
            'list<string>'   => ['list<string>', 'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX'],
            'Doc[]'          => ['Doc[]', 'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX'],
            '?Doc'           => ['?Doc', 'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX'],
            'numeric'        => ['numeric', 'ZEND_BEGIN_ARG_INFO_EX'],
            'scalar'         => ['scalar', 'ZEND_BEGIN_ARG_INFO_EX'],
            'self'           => ['self', 'ZEND_BEGIN_ARG_INFO_EX'],
            'static'         => ['static', 'ZEND_BEGIN_ARG_INFO_EX'],
            'iterable'       => ['iterable', 'ZEND_BEGIN_ARG_INFO_EX'],
            'void'           => ['void', 'ZEND_BEGIN_ARG_INFO_EX'],
            'never'          => ['never', 'ZEND_BEGIN_ARG_INFO_EX'],
            'true'           => ['true', 'ZEND_BEGIN_ARG_INFO_EX'],
            'false'          => ['false', 'ZEND_BEGIN_ARG_INFO_EX'],
            'null'           => ['null', 'ZEND_BEGIN_ARG_INFO_EX'],
            'numeric|null'   => ['numeric|null', 'ZEND_BEGIN_ARG_INFO_EX'],
        ];
    }

    private function generatedCode(string $className, string $source): string
    {
        $projectDir = $this->generateProject($className, $source);

        return (string) file_get_contents(sprintf('%s/ext/probe/%s.zep.c', $projectDir, strtolower($className)));
    }

    private function generatedHeader(string $className, string $source): string
    {
        $projectDir = $this->generateProject($className, $source);

        return (string) file_get_contents(sprintf('%s/ext/probe/%s.zep.h', $projectDir, strtolower($className)));
    }

    private function generateProject(string $className, string $source): string
    {
        $projectDir = $this->outputDir() . '/probe';
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init probe', $this->outputDir())['exitCode']);

        file_put_contents($projectDir . '/probe/' . strtolower($className) . '.zep', $source . "\n");

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stderr'] . $result['stdout']);

        return $projectDir;
    }
}
