<?php

class FunctionCache
{
	private $_cache = array();

	public function get($functionName, $compilationContext)
	{
		if (isset($this->_cache[$functionName])) {
			return $this->_cache[$functionName];
		}
		$functionCache = $compilationContext->symbolTable->getTempVariableForWrite('zend_function', $compilationContext);
		$functionCache->setMustInitNull(true);
		$functionCache->setReusable(false);
		$this->_cache[$functionName] = $functionCache;
		return $functionCache;
	}
}