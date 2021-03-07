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

namespace Zephir\Test\Documentation\Annotation;

use PHPUnit\Framework\TestCase;
use Zephir\Documentation\Annotation\See;

final class SeeTest extends TestCase
{
    private const SEE_NAME = 'see';

    public function testShouldGetLink(): void
    {
        $link = new See(self::SEE_NAME, '  sample with leading space  ');

        $this->assertSame(self::SEE_NAME, $link->getName());
        $this->assertSame(
            'sample with leading space',
            $link->getString(),
            'See annotation should be without leading spaces'
        );

        $link->setString('another link');
        $this->assertSame('another link', $link->getString());
    }

    public function testShouldGetResource(): void
    {
        $testString = 'https://github.com/zephir-lang/zephir Zephir SRC';

        $link = new See(self::SEE_NAME, $testString);

        $this->assertSame(self::SEE_NAME, $link->getName());
        $this->assertSame($testString, $link->getString());
        $this->assertSame('https://github.com/zephir-lang/zephir', $link->getResource());
    }

    public function testShouldGetText(): void
    {
        $testString = 'https://github.com/zephir-lang/zephir Zephir SRC';

        $link = new See(self::SEE_NAME, $testString);

        $this->assertSame(self::SEE_NAME, $link->getName());
        $this->assertSame($testString, $link->getString());
        $this->assertSame('Zephir SRC', $link->getText());
    }
}
