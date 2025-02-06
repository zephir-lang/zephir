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

use Zephir\Documentation\FileInterface;

use function rtrim;

use const DIRECTORY_SEPARATOR;

class Sitemap implements FileInterface
{
    protected $baseUrl;
    protected $classes;
    protected $namespaces;
    private $baseDir;

    public function __construct($baseDir, $baseUrl, $classList, $namespaceList)
    {
        $this->classes    = $classList;
        $this->namespaces = $namespaceList;
        $this->baseUrl    = rtrim($baseUrl, '\\/') . DIRECTORY_SEPARATOR;
        $this->baseDir    = $baseDir;
    }

    public function getData(): array
    {
        return [
            'classes'    => $this->classes,
            'namespaces' => $this->namespaces,
            'baseUrl'    => $this->baseUrl,
        ];
    }

    public function getOutputFile(): string
    {
        return 'sitemap.xml';
    }

    public function getTemplateName(): string
    {
        return $this->baseDir . '/Api/sitemap.php';
    }
}
