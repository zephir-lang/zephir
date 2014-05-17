<?php
/**
 * Created by PhpStorm.
 * User: bob
 * Date: 5/16/14
 * Time: 6:01 PM
 */

namespace Zephir\Documentation\File;


use Zephir\ClassDefinition;
use Zephir\Documentation\AbstractFile;
use Zephir\Documentation\File;

class ClassFile extends AbstractFile {

    /**
     * @var ClassDefinition
     */
    protected $class;

    function __construct($config , ClassDefinition $class)
    {
        $this->class = $class;
    }

    public function getTemplateName()
    {
        return "class.phtml";
    }

    public function getData()
    {
        return array(

            "className" => $this->class->getName(),
            "classNamespace" => $this->class->getNamespace(),

        );
    }

    public function getOutputFile()
    {
        return "html/class/" . str_replace("\\","/", $this->class->getCompleteName()) . ".html" ;
    }


    public function writeFile($baseDir)
    {
        parent::writeFile($baseDir, "class/" . $this->data['className'] .".html", $this->data);
    }


} 