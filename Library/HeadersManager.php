<?php

/**
 * HeadersManager
 *
 * Manages the c-headers that must be added to a file
 */
class HeadersManager
{
	protected $_headers = array();

	/**
	 *
	 * @param string $path
	 */
	public function add($path)
	{
		$this->_headers[$path] = $path;
	}

}