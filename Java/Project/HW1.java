
package ce326.hw1;
import java.util.Scanner;
/**
 *
 * @author petros
 */
public class HW1 {

    public static void main(String[] args) 
    {
        Scanner sc = new Scanner(System.in);
        String word;
        String pattern = "[a-zA-Z]+";
        TrieNode root = new TrieNode();
        Trie trie = new Trie(root);        
        String choice = "init";

            
        System.out.println("?: ");
        while(sc.hasNext())
        {
            choice = sc.next();            
            switch(choice)
            {
                case "-i": 
                    word = sc.next();
                    //check for match and lowercase
                    if(word.matches(pattern))
                    {
                        word = word.toLowerCase();

                        boolean k = trie.insert(word, root);
                        if(k  == true)
                            System.out.println("ADD " + word + " OK");
                        else
                            System.out.println("ADD " + word + " NOK");
                    }
                    break;
                    
                case "-r": 
                    word = sc.next();    
                    //check for match and lowercase
                    if(word.matches(pattern))
                    {
                        word = word.toLowerCase();           
                        if(trie.delete(word, root))
                            System.out.println("RMV " + word + " OK");
                        else
                            System.out.println("RMV " + word + " NOK");
                    }
                        
                    break;
                    
                case "-f":
                    word = sc.next();
                    //check for match and lowercase
                    if(word.matches(pattern))
                    {
                        word = word.toLowerCase();
                        if(trie.searchForWord(word, root) == null)
                            System.out.println("FND " + word + " NOK");
                        else 
                            System.out.println("FND " + word + " OK");
                    }
                    break;
                
                case "-p":
                    System.out.print("PreOrder: ");
                    trie.preorder(root);
                    System.out.println();
                    break;
                    
                case "-d":
                    System.out.println();
                    System.out.println("***** Dictionary *****");
                    trie.dictionary(root,"");
                    System.out.println();
                    break;
    
                case "-w":
                    word = sc.next();
                    String apos = sc.next(); // hope user gives a number
                    int apostash = Integer.parseInt(apos);
                    //check for match and lowercase
                    if(word.matches(pattern))
                    {
                        word = word.toLowerCase();
                        System.out.println();
                        System.out.println("Distant words of "+word+" ("+apostash+"):");
                        trie.xxx(root, "", word, apostash);
                        System.out.println();

                    }
                    break;
                
                case "-s":
                    word = sc.next();
                    //check for match and lowercase
                    if(word.matches(pattern))
                    {
                        word = word.toLowerCase();
                        System.out.println();
                        System.out.println("Words with suffix " +word+":");
                        trie.suffix(root, "", word);
                        System.out.println();

                    }                  
                    break;
                
                case "-q":
                    System.out.println("Bye bye!");
                    sc.close();
                    System.exit(0);
                    break;
                    
            }

            System.out.println("?: ");
        }
        
        
    }
}
