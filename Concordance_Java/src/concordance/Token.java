/*****************************************
 * Token:                                *
 * this class represents the concrete    *
 * implementation of a content obj       *
 * within a node stored in a data store. *
 *****************************************/
package concordance;

import java.util.ArrayList;

public class Token implements Content {
  /* String representing the token */
  private String token;
  /* Lines (from text) where the token has been found */
  private ArrayList<Integer> idxLine;
  /* Number of occurences of the token in the text */
  private int counter;
  /* Global counter (i.e., num of tokens parsed so far + 1) */
  private static int glbCounter = 0;
  /* Unique id of the token */
  private int tokenId;
  
  private void overwriteContent ( String str ) {
    token = str;
  }//overwriteContent
  
  /* Init constructor */
  private void init() {
    token   = "";
    counter = 1;
    tokenId = glbCounter++;
    idxLine = new ArrayList<Integer>();
  }//init
  
  public Token() {
    init();
  }//Token
  public Token ( String str ) {
    init();
    token = str;
  }//Token
  public Token ( String str, int line ) {
    init();
    token = str;
    idxLine.add( line );
  }//Token
  public Token ( String str, int line, int counter ) {
    init();
    token = str;
    idxLine.add( line );
    this.counter = counter;
  }//Token
  public Token ( String str, ArrayList<Integer> idxLine ) {
    init();
    token = str;
    this.idxLine = new ArrayList<Integer> (idxLine);
  }//Token
  public Token ( String str, ArrayList<Integer> idxLine, int counter ) {
    init();
    token = str;
    this.idxLine = new ArrayList<Integer> (idxLine);
    this.counter = counter;
  }//Token
  
  /* Get/Set methods*/
  public String getString() {
    return token;
  }//getString
  public ArrayList<Integer> getLines () {
    return idxLine;
  }//getLines
  
  public void setLines ( ArrayList<Integer> lines ) {
    idxLine.clear();
    idxLine = lines;
  }//getLines
  
  public int getCounter() {
    return counter;
  }//getCounter
  public int getGlbCounter() {
    return glbCounter;
  }//getGlbCounter
  public int getId() {
    return tokenId;
  }//getId
  public void setCounter ( int counter ) {
    this.counter = counter;
  }//setCounter
  
  /* Operators for string comparison  */
  @Override
  public boolean equal(Content content) {
    return token.equals( ((Token) content).getString() );
  }

  @Override
  public boolean less(Content content) {
    return (token.compareTo( ((Token) content).getString() ) < 0);
  }

  /* Check if a content matches somehow the current one */
  @Override
  public boolean match(Content content) {
    return ( token.charAt(0) == ((Token) content).getString().charAt(0) );
  }//match

  @Override
  public int getSize() {
    return token.length();
  }//getSize
  
  /*
   * Update this content with a new content:
   * if the string of the content
   * given in input is equal to this _token
   * then update the other class fields.
   * Otherwise, replace token with the string given in input. */
  @Override
  public void updateContent( Content content ) {
    if ( !equal( content ) ) {
      /* If the two strings differ: substitute the old one and return */
      overwriteContent ( ((Token) content).getString() );
      idxLine.clear();
      idxLine = ((Token) content).getLines();
      this.counter = 1;
      return;
    }
    else {
      /* Otherwise found a match: add other lines and increase counter */
      for ( int i = 0; i < ((Token) content).getLines().size(); i++ ) {
        idxLine.add( ((Token) content).getLines().get(i) );
      }
      this.counter++;
    }
  }//updateContent

  /* Check "how" a given token matches the current one.
   * This is a particular function:
   * it checkes is the match is perfect, a prefix or a suffix.
   * Moreover, it can modify the content given in input but it
   * cannot modify the current content. 
   * If it is the case, it generates a new content. */
  @Override
  public void howMatch( Content content, ArrayList<Content> matches ) {
    String otherString = ((Token) content).getString();
    if ( token.equals( otherString ) ) { return; }
    
    
    /* Find the common substring */
    String shorterStr = otherString;
    if ( token.length() < otherString.length() ) shorterStr = token;
    
    int i = 0;
    boolean differ = false;
    for ( ; i < shorterStr.length(); i++ ) {
      if ( token.charAt(i) != otherString.charAt(i) ) {
        differ = true;
        break;
      }
    }
    if ( !differ ) i++;
    i = Math.min( i, shorterStr.length() );
    String replacementStr = shorterStr.substring( 0, i );
    
    /* Here this.token is a prefix of the other string */
    if ( ( replacementStr.equals( token ) ) &&
         (token.length() < otherString.length()) ) {
      ///@note: modifies casted content not the current one!
      ((Token) content).overwriteContent( otherString.substring( token.length(), 
              token.length() + otherString.length() - token.length() ) );

      matches.add( ((Token) content) );
      return;
    }
    
    /* Here otherString is a prefix of this.token */
    if ( (replacementStr.equals( token ) ) &&
         (otherString.length() < token.length()) ) {
      
      /*Need to modify the current content:
       * create a new one.*/
      Token splitToken = new Token ( token.substring( 0, 
              Math.min(otherString.length(), token.length())), 
              ((Token) content).getLines() );
      
      matches.add( splitToken );
      
      /* It is possible to modify directly the other content */
      ((Token)content).overwriteContent(token.substring( otherString.length()));
      ((Token) content).setLines( idxLine );
      ((Token) content).setCounter( counter );
      matches.add( ((Token)content) );
      return;
    }
    
    /* New content for modifying the existing one */
    Token splitToken = new Token ( replacementStr );
    matches.add( splitToken );
    
    /* New content for its children */
    Token restOfToken = new Token ( token.substring ( i ), idxLine, counter );
    matches.add( restOfToken );

    /* Update content */
    String splitOther  = otherString.substring( i );
    ((Token) content).overwriteContent ( splitOther );
    matches.add( ((Token) content) );
  }//howMatch

  @Override
  public void print() {
    String outString = "TOKEN_" + tokenId + ":\n";
    outString += "\t String:  \"" + token   + "\"\n" +
    "\t Counter: " + counter + "\n";
    if ( idxLine.size() > 0 ) {
      outString += "\t Indeces: ";
      for ( int i = 0; i < idxLine.size() - 1; i++ )
        outString += idxLine.get(i) + ", ";
      outString += idxLine.get( idxLine.size() - 1 ) + "\n";
    }
    else {
      outString += "\t NO Indeces";
    }
    System.out.println ( outString );
  }//print

  @Override
  public void printInfo() {
    String outString = "{";
    outString += counter + ":";
    if ( idxLine.size() > 0 ) {
      for ( int i = 0; i < idxLine.size() - 1; i++ )
        outString += idxLine.get( i ) + ",";
      outString += idxLine.get(idxLine.size() - 1 );
    }
    else {
      outString += "0";
    }
    outString += "}";
    System.out.println( outString );
  }//printInfo

  @Override
  public String getInfo() {
    String outString = "{";
    outString += counter + ":";
    if ( idxLine.size() > 0 ) {
      for ( int i = 0; i < idxLine.size() - 1; i++ )
        outString += idxLine.get( i ) + ",";
      outString += idxLine.get(idxLine.size() - 1 );
    }
    else {
      outString += "0";
    }
    outString += "}\n";
    return outString;
  }//getInfo
  
  /* Get a reference to the "base" content object */
  @Override
  public Object getRefBaseObj() {
    return token;
  }
  
}//Token
