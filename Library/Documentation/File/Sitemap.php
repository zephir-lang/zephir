<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation\File;

use Zephir\Documentation\AbstractFile;

class Sitemap extends AbstractFile
{
    protected $classes;
    protected $namespaces;
    protected $baseUrl;
    private $baseDir;

    public function __construct($baseDir, $baseUrl, $classList, $namespaceList)
    {
        $this->classes = $classList;
        $this->namespaces = $namespaceList;
        $this->baseUrl = rtrim($baseUrl, '\\/').\DIRECTORY_SEPARATOR;
        $this->baseDir = $baseDir;
    }

    public function getTemplateName()
    {
        return $this->baseDir.'/Api/sitemap.php';
    }

    public function getData()
    {
        return [
            'classes' => $this->classes,
            'namespaces' => $this->namespaces,
            'baseUrl' => $this->baseUrl,
        ];
    }

    public function getOutputFile()
    {
        return 'sitemap.xml';
    }
}
