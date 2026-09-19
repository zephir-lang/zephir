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
use Zephir\Code\Builder\Struct;
use Zephir\Exception\InvalidArgumentException;

use const PHP_EOL;

/**
 * A dotted global such as `db.my_setting` lands in a nested C struct. The
 * struct only declares the member; what the member is worth and how php.ini
 * reaches it belongs to ExtensionGlobal.
 */
final class StructTest extends TestCase
{
    private function struct(): Struct
    {
        return new Struct('_zephir_struct_db', 'db');
    }

    public function testShouldRejectAnEmptyName(): void
    {
        $this->expectException(InvalidArgumentException::class);
        $this->expectExceptionMessage('Struct name must not be empty');

        new Struct('', 'db');
    }

    public function testShouldRejectADuplicateProperty(): void
    {
        $struct = $this->struct();
        $struct->addProperty('my_setting', 'int');

        $this->expectException(InvalidArgumentException::class);
        $this->expectExceptionMessage('Property was defined more than once');

        $struct->addProperty('my_setting', 'int');
    }

    public function testShouldDeclareEngineWidthMembers(): void
    {
        $struct = $this->struct();
        $struct->addProperty('a_bool', 'bool');
        $struct->addProperty('an_int', 'int');
        $struct->addProperty('a_uint', 'uint');
        $struct->addProperty('a_long', 'long');
        $struct->addProperty('a_ulong', 'ulong');
        $struct->addProperty('a_double', 'double');
        $struct->addProperty('a_char', 'char');
        $struct->addProperty('a_string', 'string');

        $expected = 'typedef struct _zephir_struct_db { ' . PHP_EOL
            . "\tzend_bool a_bool;" . PHP_EOL
            . "\tzend_long an_int;" . PHP_EOL
            . "\tzend_ulong a_uint;" . PHP_EOL
            . "\tzend_long a_long;" . PHP_EOL
            . "\tzend_ulong a_ulong;" . PHP_EOL
            . "\tdouble a_double;" . PHP_EOL
            . "\tchar a_char;" . PHP_EOL
            . "\tchar * a_string;" . PHP_EOL
            . '} zephir_struct_db;' . PHP_EOL;

        $this->assertSame($expected, (string) $struct);
    }
}
