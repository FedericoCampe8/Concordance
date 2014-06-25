/*****************************************
 * ConcordanceVisitor (concrete Visitor) *
 * this class implements a Visitor for   *
 * visiting the Prefix tree and printing *
 * the Concordance of the parsed text.   *
 **************************************/
package concordance;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Stack;

class ConcordanceVisitor implements Visitor {
  
  private String outFile;
  private String strToPrint;
  private String currentToken;
  
  /* Stack used for backtracking on print.
   * It holds: 
   * a) Size of the string
   * b) Level of the tree where the string has been found */
  private Stack<Pair> trailStack;
  
  private void init () {
    strToPrint   = "";
    currentToken = "";
    outFile      = "";
    trailStack   = new Stack<Pair>();
  }
  
  public ConcordanceVisitor() {
    init();
  }//ConcordanceVisitor
  public ConcordanceVisitor( String outFile ) {
    init();
    this.outFile = outFile;
  }//ConcordanceVisitor

  @Override
  public void visit() {
    throw new UnsupportedOperationException("Not supported yet.");
  }

  @Override
  public void visitNode( Node node ) {
    
    String currentString;
    int level = node.getLevel();
    
    /* Cases...*/
    if ( trailStack.empty() ) {
      trailStack.push( new Pair( node.getContent().getSize(), level ) );
    }
    else {
      if ( level > trailStack.peek().second() ) {
        currentString = (String)node.getContent().getRefBaseObj();
        currentToken += currentString;
        trailStack.push( new Pair ( node.getContent().getSize(), level ));
      }
      else if ( level == trailStack.peek().second() ) {
        while ( trailStack.peek().second() == level ) {
          currentToken = currentToken.substring( 
                  0, currentToken.length() - trailStack.peek().first() );
        trailStack.pop();
        }//while
        currentString = (String)node.getContent().getRefBaseObj();
        currentToken += currentString;
        trailStack.push( new Pair ( node.getContent().getSize(), level ));
      }
      else {
        while ( trailStack.peek().second() >= level ) {
          currentToken = currentToken.substring( 
                  0, currentToken.length() - trailStack.peek().first() );
          trailStack.pop();
        }
        currentString = (String)node.getContent().getRefBaseObj();
        currentToken += currentString;
        trailStack.push( new Pair ( node.getContent().getSize(), level ));
      }
    }
    
    /* Output information as desired for concordance */
    if ( node.isFinal() ) {
      if ( outFile.equals ("") ) {
        System.out.print( currentToken );
        
        for (int i = 0; i < 25 - currentToken.length(); i++ ) {
          System.out.print(" ");
        }
        ((Token) node.getContent()).printInfo();
      }
      else {
        strToPrint += currentToken;
        for ( int i = 0; i < 25 - currentToken.length(); i++ ) {
          strToPrint += " ";
        }
        strToPrint += ((Token) node.getContent()).getInfo();
      }
     }//isFinal
  }//visitNode
  
  public void printToFile() {
    try {
      BufferedWriter bw = new BufferedWriter( new FileWriter(outFile) );
      bw.write(strToPrint);
      bw.close();
    } 
    catch ( IOException e ) {
      System.out.println( "Can't open the file " + outFile );
    }
  }//printToFile
}//ConcordanceVisitor
