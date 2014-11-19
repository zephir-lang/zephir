
namespace Test;

class Trie
{
	private trie;

	public function getTrieNodeByKey(string key, boolean create = false)
	{
		var trieNode, keyLen, i, character;
		let trieNode = this->trie;
		let keyLen = strlen(key);
		let i = 0;

		while (i < keyLen) {
			let character = substr(key, i, 1);
			if !isset trieNode->children[character] {
				if create {
					let trieNode->children[character] = new \tries\trienode();
				} else {
					return false;
				}
			}
			let trieNode = trieNode->children[character];
			let i += 1;
		}
		return trieNode;
	}
}