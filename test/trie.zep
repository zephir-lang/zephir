
namespace Test;

class Trie
{
	private trie;

	public function getTrieNodeByKey(string key, create = false)
    {
        var trieNode, keyLen, i, character;
        let trieNode = this->trie;
        let keyLen = strlen(key);
        let i = 0;

		echo "key = [", key, "]", PHP_EOL;
		echo "keyLen = [", keyLen, "]", PHP_EOL;
		echo "i = [", i, "]", PHP_EOL;

        while (i < keyLen) {
			echo "In while loop", PHP_EOL;
            let character = substr(key,i,1);
            if (!isset(trieNode->children[character])) {
                if (create) {
                    let trieNode->children[character] = new tries\trienode();
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