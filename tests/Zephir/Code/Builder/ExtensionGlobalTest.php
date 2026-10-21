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

namespace Zephir\Test\Code\Builder;

use PHPUnit\Framework\TestCase;
use Zephir\Code\Builder\ExtensionGlobal;
use Zephir\Exception\InvalidArgumentException;

/**
 * One extension global, one place that decides its C type, its INI directive
 * and its compiled default. Both emitters (plain globals in Compiler and
 * dotted ones in Struct) go through this, so the two cannot drift again.
 */
final class ExtensionGlobalTest extends TestCase
{
    public function cTypeProvider(): array
    {
        return [
            'bool'    => ['bool', 'zend_bool'],
            'boolean' => ['boolean', 'zend_bool'],
            // Every integer type is engine-width: a C `int` is 4 bytes and
            // OnUpdateLong writes 8 through it. See #2666 for the Windows half.
            'int'     => ['int', 'zend_long'],
            'long'    => ['long', 'zend_long'],
            'uint'    => ['uint', 'zend_ulong'],
            'ulong'   => ['ulong', 'zend_ulong'],
            'double'  => ['double', 'double'],
            'char'    => ['char', 'char'],
            'uchar'   => ['uchar', 'unsigned char'],
            'string'  => ['string', 'char *'],
            'hash'    => ['hash', 'HashTable *'],
        ];
    }

    /**
     * @dataProvider cTypeProvider
     */
    public function testShouldResolveCType(string $type, string $expected): void
    {
        $global = new ExtensionGlobal('setting', ['type' => $type, 'default' => 0]);

        $this->assertSame($expected, $global->cType());
    }

    public function testShouldRejectAnUnknownType(): void
    {
        $this->expectException(InvalidArgumentException::class);
        $this->expectExceptionMessage('Unknown type "resource" for extension global "setting"');

        (new ExtensionGlobal('setting', ['type' => 'resource', 'default' => 0]))->cType();
    }

    public function iniEntryProvider(): array
    {
        return [
            'bool true'  => [
                ['type' => 'bool', 'default' => true],
                'STD_PHP_INI_BOOLEAN("stub.setting", "1", PHP_INI_ALL, OnUpdateBool, setting, zend_stub_globals, stub_globals)',
            ],
            'bool false' => [
                ['type' => 'bool', 'default' => false],
                'STD_PHP_INI_BOOLEAN("stub.setting", "0", PHP_INI_ALL, OnUpdateBool, setting, zend_stub_globals, stub_globals)',
            ],
            'int'        => [
                ['type' => 'int', 'default' => 10],
                'STD_PHP_INI_ENTRY("stub.setting", "10", PHP_INI_ALL, OnUpdateLong, setting, zend_stub_globals, stub_globals)',
            ],
            'long'       => [
                ['type' => 'long', 'default' => -7],
                'STD_PHP_INI_ENTRY("stub.setting", "-7", PHP_INI_ALL, OnUpdateLong, setting, zend_stub_globals, stub_globals)',
            ],
            // Unsigned types refuse a negative php.ini value instead of
            // wrapping around, which is what OnUpdateLongGEZero is for.
            'uint'       => [
                ['type' => 'uint', 'default' => 64],
                'STD_PHP_INI_ENTRY("stub.setting", "64", PHP_INI_ALL, OnUpdateLongGEZero, setting, zend_stub_globals, stub_globals)',
            ],
            'ulong'      => [
                ['type' => 'ulong', 'default' => 64],
                'STD_PHP_INI_ENTRY("stub.setting", "64", PHP_INI_ALL, OnUpdateLongGEZero, setting, zend_stub_globals, stub_globals)',
            ],
            'double'     => [
                ['type' => 'double', 'default' => 15.2],
                'STD_PHP_INI_ENTRY("stub.setting", "15.2", PHP_INI_ALL, OnUpdateReal, setting, zend_stub_globals, stub_globals)',
            ],
            'string'     => [
                ['type' => 'string', 'default' => 'custom_value'],
                'STD_PHP_INI_ENTRY("stub.setting", "custom_value", PHP_INI_ALL, OnUpdateString, setting, zend_stub_globals, stub_globals)',
            ],
            'char'       => [
                ['type' => 'char', 'default' => 'A'],
                'STD_PHP_INI_ENTRY("stub.setting", "A", PHP_INI_ALL, zephir_OnUpdateChar, setting, zend_stub_globals, stub_globals)',
            ],
            'uchar'      => [
                ['type' => 'uchar', 'default' => 'A'],
                'STD_PHP_INI_ENTRY("stub.setting", "A", PHP_INI_ALL, zephir_OnUpdateChar, setting, zend_stub_globals, stub_globals)',
            ],
        ];
    }

    /**
     * @dataProvider iniEntryProvider
     */
    public function testShouldEmitAnIniEntry(array $definition, string $expected): void
    {
        $global = new ExtensionGlobal('setting', $definition);

        $this->assertTrue($global->isIniCapable());
        $this->assertSame($expected, $global->iniEntry('stub'));
    }

    public function testShouldEmitNoIniEntryForAHash(): void
    {
        $global = new ExtensionGlobal('cache', ['type' => 'hash', 'default' => 'NULL']);

        $this->assertFalse($global->isIniCapable());
        $this->assertSame('', $global->iniEntry('stub'));
    }

    public function testShouldNameTheDirectiveAfterTheMemberPath(): void
    {
        $global = new ExtensionGlobal('db.my_setting', ['type' => 'int', 'default' => 100]);

        $this->assertSame('stub.db.my_setting', $global->iniName('stub'));
        $this->assertSame(
            'STD_PHP_INI_ENTRY("stub.db.my_setting", "100", PHP_INI_ALL, OnUpdateLong, db.my_setting, zend_stub_globals, stub_globals)',
            $global->iniEntry('stub'),
        );
    }

    public function testShouldHonourAnExplicitIniEntryBlock(): void
    {
        $global = new ExtensionGlobal('my_setting', [
            'type'      => 'bool',
            'default'   => true,
            'ini-entry' => ['name' => 'ini-entry.my_setting', 'scope' => 'PHP_INI_SYSTEM'],
        ]);

        $this->assertSame('ini-entry.my_setting', $global->iniName('stub'));
        $this->assertSame('PHP_INI_SYSTEM', $global->iniScope());
        $this->assertSame(
            'STD_PHP_INI_BOOLEAN("ini-entry.my_setting", "1", PHP_INI_SYSTEM, OnUpdateBool, my_setting, zend_stub_globals, stub_globals)',
            $global->iniEntry('stub'),
        );
    }

    /**
     * REGISTER_INI_ENTRIES() runs every on_modify handler at MINIT with the
     * php.ini value, so an INI-backed field is already seeded by the time any
     * user code runs. Assigning a compiled literal on top of that is what
     * threw the php.ini value away on every request.
     *
     * @dataProvider iniEntryProvider
     */
    public function testShouldEmitNoCompiledDefaultForAnIniBackedGlobal(array $definition): void
    {
        $global = new ExtensionGlobal('setting', $definition);

        $this->assertSame('', $global->cDefault('stub'));
    }

    public function testShouldEmitACompiledDefaultForAHash(): void
    {
        $global = new ExtensionGlobal('orm.ast_cache', ['type' => 'hash', 'default' => 'NULL']);

        $this->assertSame('stub_globals->orm.ast_cache = NULL;', $global->cDefault('stub'));
    }

    public function testShouldReportModuleScope(): void
    {
        $request = new ExtensionGlobal('a', ['type' => 'bool', 'default' => true]);
        $module  = new ExtensionGlobal('b', ['type' => 'bool', 'default' => true, 'module' => true]);

        $this->assertFalse($request->isModule());
        $this->assertTrue($module->isModule());
    }

    /**
     * OnUpdateString points a string member into its ini entry, which must
     * never be freed, so globals_set() keeps the copy it writes in a member of
     * its own (#2725).
     */
    public function testShouldNameTheOwnedCopyOfAString(): void
    {
        $global = new ExtensionGlobal('setting', ['type' => 'string', 'default' => 'value']);

        $this->assertSame('zephir_owned_setting', $global->ownedMember());
    }

    public function testShouldNameTheOwnedCopyOfAStringInAStruct(): void
    {
        $global = new ExtensionGlobal('db.setting', ['type' => 'string', 'default' => 'value']);

        $this->assertSame('db.zephir_owned_setting', $global->ownedMember());
    }

    public function testShouldStartTheOwnedCopyEmpty(): void
    {
        $global = new ExtensionGlobal('db.setting', ['type' => 'string', 'default' => 'value']);

        $this->assertSame('stub_globals->db.zephir_owned_setting = NULL;', $global->ownedInit('stub'));
    }

    public function testShouldFreeTheOwnedCopy(): void
    {
        $global = new ExtensionGlobal('db.setting', ['type' => 'string', 'default' => 'value']);

        $this->assertSame(
            'zephir_globals_release_string(&stub_globals->db.zephir_owned_setting);',
            $global->ownedRelease('stub'),
        );
    }

    public function notOwnedProvider(): array
    {
        return [
            'bool'   => ['bool'],
            'int'    => ['int'],
            'double' => ['double'],
            'char'   => ['char'],
            'hash'   => ['hash'],
        ];
    }

    /**
     * @dataProvider notOwnedProvider
     */
    public function testShouldOwnNothingForAnyOtherType(string $type): void
    {
        $global = new ExtensionGlobal('setting', ['type' => $type, 'default' => 0]);

        $this->assertSame(
            [null, '', ''],
            [$global->ownedMember(), $global->ownedInit('stub'), $global->ownedRelease('stub')],
        );
    }

    public function validNameProvider(): array
    {
        return [
            // One character used to be rejected: the check matched the
            // character class against the whole name, inverted.
            'one character'           => ['x'],
            'one character in struct' => ['a.b'],
            'plain'                   => ['my_setting'],
            'leading underscore'      => ['_private'],
            'digits after the first'  => ['cache2'],
            'struct member'           => ['orm.cache_level'],
        ];
    }

    /**
     * @dataProvider validNameProvider
     */
    public function testShouldAcceptAName(string $name): void
    {
        ExtensionGlobal::assertValidName($name);

        $this->addToAssertionCount(1);
    }

    public function invalidNameProvider(): array
    {
        $identifier = 'must be a C identifier (letters, digits and underscores, not starting with a digit)';

        return [
            'dash'                => ['my-setting', '"my-setting" ' . $identifier],
            'leading digit'       => ['1abc', '"1abc" ' . $identifier],
            'dash in a member'    => ['db.my-setting', '"my-setting" ' . $identifier],
            'leading digit field' => ['db.1x', '"1x" ' . $identifier],
            'empty'               => ['', '"" ' . $identifier],
            'empty member'        => ['db.', '"" ' . $identifier],
            'empty struct'        => ['.db', '"" ' . $identifier],
            // Everything past the second dot used to be dropped, so these two
            // were the same member.
            'two levels'          => ['a.b.c', 'only "member" or "struct.member" is supported'],
            'owned prefix'        => ['zephir_owned_str', 'the "zephir_owned_" prefix is reserved'],
            'owned prefix field'  => ['db.zephir_owned_str', 'the "zephir_owned_" prefix is reserved'],
            'kernel member'       => ['initialized', '"initialized" is a member the Zephir kernel declares'],
            'kernel member struct' => ['scache.x', '"scache" is a member the Zephir kernel declares'],
        ];
    }

    /**
     * @dataProvider invalidNameProvider
     */
    public function testShouldRejectAName(string $name, string $reason): void
    {
        $this->expectException(InvalidArgumentException::class);
        $this->expectExceptionMessage(sprintf('Extension global "%s" is not a valid name: %s', $name, $reason));

        ExtensionGlobal::assertValidName($name);
    }

    public function testShouldRequireADefault(): void
    {
        $this->expectException(InvalidArgumentException::class);
        $this->expectExceptionMessage('Extension global "setting" has no default value');

        /* The constructor rejects it, so cType() is never reached. */
        (new ExtensionGlobal('setting', ['type' => 'int']))->cType();
    }
}
