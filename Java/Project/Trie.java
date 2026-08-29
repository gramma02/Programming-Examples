
package ce326.hw1;

//import javax.lang.model.util.ElementScanner6;

/**
 *
 * @author petros
 */
public class Trie 
{
    TrieNode root; //thats the rrot

    //contructor
    public Trie(TrieNode initRoot) //willbe init with new TrieNode
    {
        root = initRoot;
    }
    
    //second implementation for simpler shit
    public Trie()
    {
        root = new TrieNode();
    }

    
    //methods
    void preorder(TrieNode node)
    {
        if(node != null) //for the children
        {
            if(node.str != null) //basically only for the root
            {    
                System.out.print(node.str); // node
                if(node.isTerminal) 
                    System.out.print("#"); // terminal
                System.out.print(" ");
            } 

            for(int i=0; i<26; i++)
                preorder(node.children[i]);
        }
    }


    void dictionary(TrieNode node, String temp) //you need an empty string here
    {
        if(node.isTerminal) //root wont be
            System.out.println(temp); //terminal so print the word

        for(int i=0; i<26; i++) //traverse the children
        {
            if(node.children[i] != null)
            {
                temp = temp.concat(node.children[i].str); // construct the string
                dictionary(node.children[i], temp);
                temp = temp.substring(0, (temp.length() - node.children[i].str.length())); //reduse the string
            }
        }  
    }


    // "a" typecasted to int is 97, so substruct it from the char!
    TrieNode searchForWord(String word, TrieNode node) //in the trienode the root must be given
    {
        int i=0;
        boolean strWasted = false;
        boolean wordWasted = false;

        // how many same on prefix???
        if(node.str != null) // if im not at the rootcharAtcharAt
        {
            while(i < word.length() && i < node.str.length()) //check strcharAting size
            {
                if(word.charAt(i) == node.str.charAt(i))
                    i++;
                else
                    break;
            }

            // 4 cases!
            if(i == node.str.length())
                strWasted = true;
            if(i == word.length())
                wordWasted = true;
            
        } // we now know how many matching characters there are
        else // we at the root, root str is null so consider it wasted
            strWasted = true; 


        // check whats happening
        if(wordWasted)
            if(strWasted) // 1,1
                if(node.isTerminal)
                    return node; // found
                else
                    return null; // its here but non Terminal
            else // 1,0
                return null; // string not wasted, our word is a subword
        else // word NOT wasted 
            if(strWasted) // word is bigger and matching, need to check children : 0,1
            {
                word = word.substring(i); // keep the diif char as the word
                if(node.children[((int)word.charAt(0)- 97)] != null) // if there is tree to traverse: GO
                    return searchForWord(word, node.children[((int)word.charAt(0)- 97)]);
                else
                    return null;
            }
            else // str NOT wasted(NOT matching characters): 0,0
                return null;

    }
    
    boolean delete(String word, TrieNode node)
    {
        TrieNode k = searchForWord(word, node); 
        if(k == null)
            return false;
        else // word found
        {
            // how many children does the node have???
            int j = 0;
            int l = 0;
            for(int i=0; i<26; i++)
            {
                if(k.children[i] != null)
                {
                    if(j == 0)
                        l = i; // keep the position of the first child
                    j++;
                }
            }
            // j==0 here
            if(j == 0) // either raw delete or also merge his father with the one and only child left
            {
                // count his parents children
                int m = 0;
                int pos = 0;
                for(int n=0; n<26; n++)
                {
                    if(k.parent.children[n] != null && (n != (int)k.str.charAt(0) - 97)) //keep f other than self
                    {
                        //System.out.print(n +" ");
                        if(m == 0)
                            pos = n;

                        m++;

                    }
                }
                //if his parent has only one kid (other than the deleting one) and terminal merge and kill again
                //if kid not terminal check his children and if only one then merge
                if(m == 1 && k.parent.str != null) // second check for root
                {
                 
                    k.parent.children[pos].str = k.parent.str.concat(k.parent.children[pos].str); // construct brothers string
                    k.parent.children[pos].parent = k.parent.parent; //grandpa is the new father
                    k.parent.parent.children[(int)k.parent.children[pos].str.charAt(0) - 97] = k.parent.children[pos]; //get the grandpas ptr to the deleted nodes bro
                    // now there is no pointer to the k node so it must be deleted by garbage collector
                   
                }// else only the delete of the node is enough
                
                //DELEEETE
                k.parent.children[(int)k.str.charAt(0) - 97] = null;
                k.parent = null;

                return true;
    
            }
            else if(j == 1) //has one child, merge it with it
            {
                //case klhronomei ta paidia toy paidioy
                k.children[l].str = k.str.concat(k.children[l].str); 
                k.children[l].parent = k.parent;
                k.parent.children[k.children[l].str.charAt(0) - 97] = k.children[l];
                k.parent = null;
                return true;
            }
            else // polyteknos, you cant kill him
            {
                k.isTerminal = false;
                return true;
            }
        
        }
    }

    boolean insert(String word, TrieNode node)
    {
        int i=0;
        boolean strWasted = false;
        boolean wordWasted = false;

        // how many same on prefix???
        if(node.str != null) // if im not at the root
        {
            while(i < word.length() && i < node.str.length()) //check string size
            {
                if(word.charAt(i) == node.str.charAt(i))
                    i++;
                else
                    break;
            }

            // 4 cases!
            if(i == node.str.length())
                strWasted = true;
            if(i == word.length())
                wordWasted = true;
            
        } // we now know how many matching characters there are
        else // we at the root, root str is null so consider it wasted
            strWasted = true; 

        // GO
        if(strWasted)
        {
            
            if(wordWasted) // 1,1
            {
                if(node.isTerminal)
                    return false; //found
                else
                {
                    node.setAsTerminal();
                    return true;
                }
            }
            else // word NOT wasted 0,1
            {
                word = word.substring(i); // keep the diif char as the word
                if(node.children[((int)word.charAt(0)- 97)] != null) // if there is tree to traverse: GO
                    return insert(word, node.children[((int)word.charAt(0)- 97)]);
                else // make new node
                {
                    TrieNode neo = new TrieNode(word,true, node);
                    node.children[((int)word.charAt(0)- 97)] = neo;
                    return true;
                }
            }
        }
        else // str NOT wasted
        {
            // if str NOT wasted we manage only this and make the prefix terminal if word is wasted : 1,0
            TrieNode prefix = new TrieNode(word.substring(0, i), wordWasted, node.parent); // keep prefix on new node
            node.str = node.str.substring(i); // cut the common letters from node word
            node.parent.children[(int)prefix.str.charAt(0) - 97] = prefix; // make it child you fool
            prefix.children[(int)node.str.charAt(0) - 97] = node; // make the cut node the child of the new one
            node.parent = prefix; // make prefix nodes parent
            
            if(wordWasted == false) // Word not wasted : 0,0
            {
                word = word.substring(i); // keep the unmatced letters
                TrieNode suffix = new TrieNode(word, true, prefix); // make a new node for the suffix
                prefix.children[(int)word.charAt(0) - 97] = suffix; // put it as the prefix child
            }

            return true;

        }

    }



    void xxx(TrieNode node, String temp, String word, int apostash) //you need an empty string here
    {
        if(node.isTerminal) //root wont be
        {
            if(temp.length() == word.length())
            {
                int j=0;
                for(int i=0; i<word.length(); i++) //find the common characters
                    if(word.charAt(i) == temp.charAt(i))
                        j++;

                if((word.length() - j) == apostash)
                    System.out.println(temp); // terminal so print the word

            }
            
        }

        for(int i=0; i<26; i++) //traverse the children
        {
            if(node.children[i] != null)
            {
                temp = temp.concat(node.children[i].str); // construct the string
                xxx(node.children[i], temp, word, apostash);
                temp = temp.substring(0, (temp.length() - node.children[i].str.length())); //reduse the string
            }
        }  
    }

    void suffix(TrieNode node, String temp, String word) //word is the suffix
    {
        if(node.isTerminal) // root and building nodes wont be
        {   
            if(word.length() <= temp.length())
            {
                int j=0; // common suffix letters 
                for(int i=temp.length()-word.length(); i<temp.length(); i++) //find the common characters
                    if(word.charAt(i - (temp.length()-word.length())) == temp.charAt(i))
                        j++;
            
                if((word.length() - j) == 0)
                    System.out.println(temp); // full match so print
            }
            
        }

        for(int i=0; i<26; i++) //traverse the children
        {
            if(node.children[i] != null)
            {
                temp = temp.concat(node.children[i].str); // construct the string
                suffix(node.children[i], temp, word);
                temp = temp.substring(0, (temp.length() - node.children[i].str.length())); // reduse the string
            }
        }
        
    }
}
