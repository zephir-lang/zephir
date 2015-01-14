<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Documentation;

use Zephir\Documentation\Annotation;
use Zephir\Documentation\Docblock;

/**
 * Summary
 *
 * Details on 
 * many
 * 
 * lines
 * 
 * @link http://somelink.com/ example
 * @link http://somelink2.com/ example2
 * 
 */
class AnnotationParser
{


    protected $annotation;
    protected $annotationLen;


    protected $currentChar = null;
    protected $currentCharIndex = null;
    protected $currentLine = null;
    protected $lines = array();
    
    
    // Parsing helpers
    protected $ignoreSpaces;
    protected $ignoreStar;
    protected $commentOpen;
    protected $annotationNameOpen;
    protected $annotationOpen;
    protected $summaryOpen;
    protected $descriptionOpen;

    // parsed data
    protected $currentAnnotationStr;
    protected $currentAnnotationContentStr;
    protected $summaryStr;
    protected $descriptionStr;
    /**
     *
     * @var Docblock
     */
    protected $docblockObj;


    /**
     * 
     * @param string $annotation a row annotation string begining with /**
     */
    function __construct($annotation) {
        $this->annotation = $annotation;
    }

    /**
     * Parses the internal annotation string
     */
    public function parse(){
  
        $this->docblockObj = new Docblock();
        
        $this->ignoreSpaces = false;
        $this->ignoreStar   = true;
        $this->commentOpen  = false;
        $this->annotationNameOpen = false;
        $this->annotationOpen = false;
        $this->summaryOpen = true;
        $this->descriptionOpen = false;
        
        $this->currentAnnotationStr = null; 
        $this->currentAnnotationContentStr = null; 
        
        $this->summaryStr = "";
        $this->descriptionStr = "";
        
        $this->currentLine = 0;
        $this->currentCharIndex = 0;
        $this->annotationLen = strlen($this->annotation);
        $this->currentChar = $this->annotation[0];

        while( null !== $this->currentChar){
            
            $currentChar = $this->currentChar;

            if($this->ignoreSpaces && ctype_space($currentChar));
            else{
                if(!$this->commentOpen){
                    if($currentChar == "/"){
                        if($this->nextCharacter() == "*" && $this->nextCharacter() == "*"){
                            $this->commentOpen = true;
                        }else{
                            continue;
                        }
                    }
                }else{
                    
                    if($currentChar=="*"){
                        if($this->nextCharacter() == "/"){
                            // stop annotation parsing on end of comment
                            $this->tryRegisterAnnotation();
                            break;
                        }else if($this->ignoreStar){
                            $this->nextCharacter();
                            continue;
                        }
                    }
                    
                    if($currentChar=="@"){
                        
                        $this->descriptionStr = trim($this->descriptionStr);
                        if($this->descriptionOpen && strlen($this->descriptionStr) > 0){
                            $this->descriptionOpen = false;
                        }

                        $this->currentAnnotationStr = "";
                        $this->currentAnnotationContentStr = "";
                        
                        $this->ignoreSpaces = false;
                        $this->annotationNameOpen = true;
                        
                    }else if($this->annotationNameOpen || $this->annotationOpen){
                        
                        // stop annotation parsing on new line
                        if( $currentChar == "\n" || $currentChar == "\r"){

                            $this->tryRegisterAnnotation();
                            
                            $this->ignoreSpaces = true;
                            $this->ignoreStar   = true;
                            
                            

                        }else if($this->annotationNameOpen){

                            if( ctype_space($currentChar) ){
                                $this->annotationNameOpen = false;
                                $this->annotationOpen = true;
                            }else{
                                $this->currentAnnotationStr .= $currentChar;
                            }

                        }else if($this->annotationOpen){
                            $this->currentAnnotationContentStr .= $currentChar;
                        }
                        
                    }else if($this->summaryOpen){
                        
                        // stop summary on new line
                        if( strlen($this->summaryStr) > 0 && ($currentChar == "\n" || $currentChar == "\r") ){
                            $this->summaryOpen = false;
                            $this->descriptionOpen = true;
                            $this->ignoreStar  = true;
                        }else{
                            $this->summaryStr .= $currentChar;
                        }
                        
                    }else if($this->descriptionOpen){
                        
                        // stop description on new line
                        if( $currentChar == "\n" || $currentChar == "\r" ){
                            $this->descriptionStr .= PHP_EOL;
                        }else{
                            $this->descriptionStr .= $currentChar;
                        }
                        
                    }
                    
                }
            }
            
            
            
            
            $this->nextCharacter();
        }
        
        
        $this->docblockObj->setSummary(trim($this->summaryStr));
        $this->docblockObj->setDescription(trim($this->descriptionStr));
        
    }
    
    /**
     * check if there is a currently parsed annotation, registers it, and stops the current annotation parsing
     */
    private function tryRegisterAnnotation(){
        
        if( ($this->annotationNameOpen || $this->annotationOpen) && strlen($this->currentAnnotationStr) > 0 ){
            $this->docblockObj->addAnnotation(new Annotation($this->currentAnnotationStr,$this->currentAnnotationContentStr));
        }
        
        $this->annotationNameOpen = false;
        $this->annotationOpen = false;
        
    }
    
    
    /**
     * moves the current cursor to the next character
     * @return string the new current character
     */
    private function nextCharacter(){
        
        $this->currentCharIndex++;

        if($this->annotationLen <= $this->currentCharIndex)
            $this->currentChar = null;
        else
            $this->currentChar = $this->annotation{$this->currentCharIndex};
        
        return $this->currentChar;
    }

}