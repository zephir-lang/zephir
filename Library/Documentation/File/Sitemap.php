<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Documentation\File;

use Zephir\Documentation\AbstractFile;

class Sitemap extends AbstractFile
{
    protected $classes;
    protected $namespaces;
    protected $baseUrl;

    public function __construct($baseUrl, $classList, $namespaceList)
    {
        $this->classes = $classList;
        $this->namespaces = $namespaceList;
        $this->baseUrl = rtrim($baseUrl, "/") . "/";
    }

    public function getTemplateName()
    {
        return ZEPHIRPATH . "/templates/Api/sitemap.php";
    }

    public function getData()
    {
        return array(
            "classes"    => $this->classes,
            "namespaces" => $this->namespaces,
            "baseUrl"    => $this->baseUrl,
        );
    }

    public function getOutputFile()
    {
        return "sitemap.xml";
    }
}
