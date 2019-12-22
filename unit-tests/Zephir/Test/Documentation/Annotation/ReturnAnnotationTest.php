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
use Zephir\Documentation\Annotation\ReturnAnnotation;

class ReturnAnnotationTest extends TestCase
{
    private const ANNOTATION_NAME = 'zephir return annotation name';

    /** @test */
    public function shouldGetReturnAnnotation()
    {
        $link = new ReturnAnnotation(self::ANNOTATION_NAME, '  sample with leading space  ');

        $this->assertSame(self::ANNOTATION_NAME, $link->getName());
        $this->assertSame(
            'sample with leading space',
            $link->getString(),
            'Return types annotattion should be without leading spaces'
        );

        $link->setString('array - Example description');
        $this->assertSame('array - Example description', $link->getString());
    }

    /** @test */
    public function shouldGetReturnType()
    {
        $testString = 'Zephir\EventInterface - Sample Event';

        $link = new ReturnAnnotation(self::ANNOTATION_NAME, $testString);

        $this->assertSame(self::ANNOTATION_NAME, $link->getName());
        $this->assertSame($testString, $link->getString());
        $this->assertSame('Zephir\EventInterface', $link->getReturnType());
    }

    /** @test */
    public function shouldGetLinkText()
    {
        $testString = 'Zephir\EventInterface - Sample Event';

        $link = new ReturnAnnotation(self::ANNOTATION_NAME, $testString);

        $this->assertSame(self::ANNOTATION_NAME, $link->getName());
        $this->assertSame($testString, $link->getString());
        $this->assertSame('- Sample Event', $link->getDescription());
    }
}
