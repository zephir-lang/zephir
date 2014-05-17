<?php
/**
 * Created by PhpStorm.
 * User: bob
 * Date: 5/16/14
 * Time: 5:44 PM
 */

namespace Zephir\Documentation;


use Zephir\Config;

abstract class AbstractFile {

    abstract public function getTemplateName();

    abstract public function getOutputFile();

    abstract public function getData();

} 