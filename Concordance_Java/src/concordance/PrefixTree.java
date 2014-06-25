/**********************************************
 * Prefix_tree:                               *
 * this class implements a Prefix tree.       *
 * The tree starts from the root, a node      *
 * representing an empty string or content).  *
 * Then, nodes are pushed down the tree       *
 * with the method "add_content".             *
 * Nodes are inserted by insertions and/or    *
 * splitting.                                 *
 * Each node holds a linked list of sorted    *
 * children (i.e., ordered using the ordering *
 * relation defined on the contents).         *
 * Visiting the tree (e.g., using a visitor), *
 * the content of all the tree is printed     *
 * following the sorting of the nodes.        *
 * @note: all operations (insertion, lookup,  *
 * etc.) are performed in O(k) time where k   *
 * is the maximum length of the strings in    *
 * the set. Visiting the tree takes time      *
 * O(nk), where n is the number of the        *
 * strings in the set. Space is O(nk).        *
 **********************************************/
package concordance;

class PrefixTree extends AbstractDataStore {
  
  private int nodeCounter;
  private String dbg;
  /* Root of the tree: a node*/
  private Node root;
  
  private void init() {
   dbg  = "#log: Prefix_tree - ";
   root = new Node();
   nodeCounter = 1;
  }//init
  
  private void visit_tree() {
    if ( verbose ) {
      System.out.println( dbg + "Start visiting PrefixTree");
      System.out.println( dbg + "Creatinr a visitor...");
    }
    /* Create a visitor */
    ConcordanceVisitor concordanceVisitor = 
            new ConcordanceVisitor ( outFile );
    
    if ( verbose ) {
      System.out.println( dbg + "...visitor created");
      System.out.println( dbg + "Visiting PrefixTree...");
      System.out.println( "---------------- CONCORDANCE ------------------");
    }
    
    if ( timer ) setTime();
    
    root.accept( concordanceVisitor );
    
    /* Print to file */
    if ( !outFile.equals( "" ) ) {
      concordanceVisitor.printToFile ();
    }
    
    if ( verbose ) {
      System.out.println( "-----------------------------------------------" );
    }
    if ( timer ) {
      System.out.print( dbg + "Time to visit PrefixTree: ");
      System.out.println( getTime() + "ms" );
    }
  }//visit_tree
  
  
  public PrefixTree () {
    init();
  }//PrefixTree
  
  /* Size of the tree:
   * number of nodes in the tree. */
  @Override
  public int getSize() {
    return nodeCounter;
  }//getSize
  
  /* Add content to the tree.
   * It pushes down the content starting from
   * the root of the tree. */
  @Override
  public void addContent( Content content ) {
    if ( content != null ) {
      /* System.out.println ( (String)(content.getRefBaseObj()) ); */
      root.addContent ( content );
    }
  }//addContent
  
  /* Fill Prefix tree using a parser */
  @Override
  void fillStore ( Parser prs ) {
    
    if ( verbose ) {
      System.out.println( dbg + "Start filling store...");
    }
    
    if ( timer ) setTime();
    
    root.addContent( new Token("") );
    while ( prs.hasMoreElements() ) {
      addContent ( prs.getNextElement() );
    }
    
    if ( timer ) {
      System.out.print( dbg + "Time to fill the tree: ");
      System.out.println( getTime() + "ms" );
    }
    
    if ( verbose ) {
      System.out.println( dbg + "...Filled store.");
    }
    
  }//fillStore
  
  /* Visit tree (it prints the sorted list of
   * the contents of the nodes). */
  @Override
  public void print() {
    visit_tree();
  }//print
  
  /* Prints data structure.
   * In this case, it prints the tree, 
   * i.e., nodes info. */
  @Override
  public void print_data_store () {
    root.print();
  }//print_data_store

}//PrefixTree
