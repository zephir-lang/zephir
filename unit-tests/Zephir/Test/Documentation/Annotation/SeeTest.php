<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Documentation\Annotation\See;

class SeeTest extends TestCase
{
    private const SEE_NAME = 'zephir see name';

    /** @test */
    public function shouldGetLink()
    {
        $link = new See(self::SEE_NAME, '  sample with leading space  ');

        $this->assertSame(self::SEE_NAME, $link->getName());
        $this->assertSame(
            'sample with leading space',
            $link->getString(),
            'See Annotation should be without leading spaces'
        );

        $link->setString('another link');
        $this->assertSame('another link', $link->getString());
    }

    /** @test */
    public function shouldGetResource()
    {
        $testString = 'https://github.com/cphalcon/zephir Zephir SRC';

        $link = new See(self::SEE_NAME, $testString);

        $this->assertSame(self::SEE_NAME, $link->getName());
        $this->assertSame($testString, $link->getString());
        $this->assertSame('https://github.com/cphalcon/zephir', $link->getResource());
    }

    /** @test */
    public function shouldGetText()
    {
        $testString = 'https://github.com/cphalcon/zephir Zephir SRC';

        $link = new See(self::SEE_NAME, $testString);

        $this->assertSame(self::SEE_NAME, $link->getName());
        $this->assertSame($testString, $link->getString());
        $this->assertSame('Zephir SRC', $link->getText());
    }
}
