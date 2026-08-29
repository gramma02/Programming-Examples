
package ce326.hw1;

/**
 *
 * @author petros
 */
public class TrieNode 
{
    String str;
    TrieNode [] children; // ptrs to letters
    boolean isTerminal;
    TrieNode parent; // ptr to the parent

    //root cntrusctr
    public TrieNode()
    {
        this.str = null;
        this.children = new TrieNode[26];
        this.isTerminal = false;
        this.parent = null;

    }

    //inside constructor
    public TrieNode(String initStr, boolean initIsTerm, TrieNode initParent)
    {
        this.str = initStr;
        this.children = new TrieNode[26];
        this.isTerminal = initIsTerm;
        this.parent = initParent;

    }
    
    //methods
    String getStr()
    {
        return str;
    }

    void setStr(String myStr)
    {
        this.str = myStr;
    }

    boolean terminalNode()
    {
        return this.isTerminal;
    }
    
    void setAsTerminal()
    {
        this.isTerminal = true;
    }
}

