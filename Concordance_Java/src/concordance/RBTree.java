/**************************************************
 * Red Black Tree:                                *
 * this class implement a RB-Tree together        *
 * with the operations for inserting nodes in it. *
 * RB-Tree is implemented using a TreeMap.        *
 * The key of the elements are the tokens while   *
 * the values stored are the infos about the      *
 * tokens returned by the parser.                 *
 * Operations are performed in O(log n) time      *
 * where n is the number of tokens in the set.    *
 * Filling the tree takes time O(n log n).        *
 * Space is O(n log n).                           *
 * Printing the elements of the tree takes        *
 * O(n) time and the elements are printed         *
 * following the ordering used for insertion.     *
 **************************************************/
package concordance;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.TreeMap;

/**
 *
 * @author fedecampe
 */
class RBTree extends AbstractDataStore {
  private String dbg;
  /* map representing the tree */
  private TreeMap< String, ArrayList<Content> > rbTree;
  
  public RBTree () {
    dbg  = "#log: RB_Tree - ";
    rbTree = new TreeMap< String, ArrayList<Content> >();
  }//RBTree

  /* Fill RB-tree using a parser */
  @Override
  void fillStore(Parser prs) {
    if ( verbose ) {
      System.out.println( dbg + "Start filling store...");
    }
    
    if ( timer ) setTime();
    
    /* Invoke parser to get all the
     * contents to add (on-line) into the map. */
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
  
   /* Size of the tree:
    * number of nodes in the tree. */
  @Override
  public int getSize() {
    return rbTree.size();
  }//getSize
  
   /* Add content to the tree.
    * Inset a new string with the info
    * specified in the content.
    * Otherwise update the content if the key is
    * already present. */
  @Override
  public void addContent( Content content ) {
    if ( content != null ) {
      String token = (String) content.getRefBaseObj();
      if ( rbTree.containsKey( token ) ) {
        rbTree.get( token ).add( content );
      }
      else {
        ArrayList<Content> arrAux = new ArrayList<Content>();
        arrAux.add(content);
        rbTree.put( token , arrAux );
      }
    }
  }//addContent

   /*Visit tree (it prints the sorted list of
    * the contents of the nodes). */
  @Override
  public void print() {
    
    if ( verbose ) {
      System.out.println( dbg + "Start visiting RB-Tree");
      System.out.println( dbg + "Visiting RB-Tree...");
      System.out.println( "---------------- CONCORDANCE ------------------");
    }
    
    if ( timer ) setTime();
    
    boolean firstPrint;
    String strToPrint = "";
    for ( String key: rbTree.keySet() ) {
      firstPrint = true;
      ArrayList<Content> currentCnt = rbTree.get( key );
      for ( int i = 0; i < currentCnt.size(); i++ ) {
        if ( firstPrint ) {
          strToPrint += key;
          for ( int ii = 0; ii < (25 - (key.length())); ii++ ) strToPrint += " ";
          strToPrint += "{";
          strToPrint += currentCnt.size();
          strToPrint += ":";
          firstPrint = false;
        }
        
        Content content = currentCnt.get(i);
        ArrayList<Integer> idxLine = ((Token)content).getLines();
        for ( int ii = 0; ii < idxLine.size(); ii++ ) {
          if ( i < currentCnt.size() - 1 ) {
            strToPrint += idxLine.get(ii) + ",";
          } 
          else {
            strToPrint += idxLine.get(ii);
          }
        }//ii 
      }//i
      strToPrint += "}\n";
    }//key
    
    if ( outFile.equals("") ) {
      System.out.append(strToPrint);
    }
    else {
      try {
        BufferedWriter bw = new BufferedWriter( new FileWriter(outFile) );
        bw.write(strToPrint);
        bw.close();
      } 
     catch ( IOException e ) {
        System.out.println( "Can't open the file " + outFile );
      }
    }
    
    if ( verbose ) {
      System.out.println( "-----------------------------------------------" );
    }
    if ( timer ) {
      System.out.print( dbg + "Time to visit RB-Tree: ");
      System.out.println( getTime() + "ms" );
    }
    
  }//print
  
}//RBTree
