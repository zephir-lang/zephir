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

            "classDefinition" => $this->class,
            "className" => $this->class->getName(),
            "classNamespace" => $this->class->getNamespace(),
            "fullName"  => $this->class->getCompleteName(),
            "methods"   => $this->class->getMethods()

        );
    }

    public function getOutputFile()
    {
        return "class/" . str_replace("\\","/", $this->class->getCompleteName()) . ".html" ;
    }


    public function writeFile($baseDir)
    {
        parent::writeFile($baseDir, "class/" . $this->data['className'] .".html", $this->data);
    }


} 