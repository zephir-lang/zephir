<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Documentation\Annotation;

class AnnotationTest extends TestCase
{
    /** @test */
    public function shouldGetAnnotation()
    {
        $annotation = new Annotation('test annotation', '  sample with leading space  ');

        $this->assertSame('test annotation', $annotation->getName());
        $this->assertSame(
            'sample with leading space',
            $annotation->getString(),
            'Annotation should be without leading spaces'
        );

        $annotation->setString('another annotation');
        $this->assertSame('another annotation', $annotation->getString());
    }
}
