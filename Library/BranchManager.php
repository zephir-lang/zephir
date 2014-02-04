<?php

class BranchManager
{
	protected $_currentBranch;

	public function addBranch($branch)
	{
		if ($this->_currentBranch) {

		} else {
			$this->_currentBranch = $branch;
		}
	}	
}