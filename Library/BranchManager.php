<?php

class BranchManager
{
	protected $_currentBranch;

	protected $_level = 0;

	public function addBranch(Branch $branch)
	{
		if ($this->_currentBranch) {
			$branch->setParentBranch($this->_currentBranch);
			$this->_currentBranch = $branch;
		} else {
			$this->_currentBranch = $branch;
		}
		$branch->setLevel($this->_level);
		$this->_level++;
	}

	public function getCurrentBranch()
	{
		return $this->_currentBranch;
	}

	public function removeBranch(Branch $branch)
	{
		$parentBranch = $branch->getParentBranch();
		$this->_currentBranch = $parentBranch;
		$this->_level--;
	}
}