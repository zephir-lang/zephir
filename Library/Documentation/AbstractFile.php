<?php

namespace Zephir\Documentation;


use Zephir\Config;

abstract class AbstractFile {

    abstract public function getTemplateName();

    abstract public function getOutputFile();

    abstract public function getData();
    
} 