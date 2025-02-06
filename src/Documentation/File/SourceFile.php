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

namespace Zephir\Documentation\File;

use Zephir\Class\Definition\Definition;
use Zephir\Documentation;

class SourceFile extends ClassFile
{
    protected Definition $class;

    public function getOutputFile(): string
    {
        return Documentation::sourceUrl($this->class);
    }

    public function getTemplateName(): string
    {
        return 'source.phtml';
    }
}
