/****************************************************
 * Parser class:                                    *
 * Singleton class used for parsing the input file  *
 * given by the user.                               *
 * It has methods to tokenize a text and give them  *
 * back to the client.                              *
 ****************************************************/
package concordance;

import java.io.IOException;
import java.io.RandomAccessFile;

class Parser {
  /* Static instance for singleton pattern */
  private static Parser instance = null;
  /*  Input stream (from file) */
  RandomAccessFile iFile;
  /* Tokenizator */
  Tokenization tokenizer;
  /* Other members */
  private String  inPath;
  private boolean openFile;
  private boolean openFirstTime;
  private boolean moreElements;
  private boolean newLine;
  /* String token returned by Tokenizer */
  private String token;
  /* Line read from file (i.e., readLine) */
  private String parsedLine;
  /* "Semantic" number of lines read so far */
  private int currentLine;
  /* Delimiter to use to tokenize words */
  private String delimiters;
  /* Current position within the file */
  private long currPos;
  
  private void init( String inStr ) {
    inPath        = inStr;
    openFile      = false;
    openFirstTime = true;
    moreElements  = true;
    newLine       = true;
    currentLine   = -1;
    token         = null;
    parsedLine    = null;
    delimiters    = " ";
    currPos       = 0;
  }//init
  
  protected Parser ( String inPath ) {
    init( inPath );
  }//Parser
  
  /* Static "get" singleton object */
  public static Parser getInstance( String inStr ) {
    if ( instance == null ) {
      instance = new Parser( inStr );
    }
    return instance;
  }//getInstance
  
  /* Add delimiter to tokenizer.
   * @todo: add possibility to add
   * delimiters from command line. */
  public void addDelimiter ( String del ) {
    delimiters += del;
  }//add_delimiter
  
  /* Check if the internal status has more tokens to
   * give back to the client. */
  public boolean hasMoreElements() {
    return moreElements;
  }//hasMoreElements
  
  public int getCurrentLine () {
    return currentLine;
  }//getCurrentLine
  
  /* Open the file.
   * The file is open (if not already open) and the pointer is placed
   * on the last position read.
   * If the file is open for the first time,
   * the pointer is placed on the first position.
   */
  public void open () {
    if (!openFile ) {
      try {
        iFile = new RandomAccessFile ( inPath, "r" );
      } 
      catch ( IOException e ) {
        /* Some problems here. 
         * @note: we set more elements to false instead of 
         * exiting the program.
         * For example, it may happen the case where the file
         * is moved between a series of open() - close() calls.
         * The client wants to preserve the part of the file
         * already read.
         */
        System.out.println( "Can't open the file " + inPath );
        moreElements = false;
        return;
      }
    }
    if ( openFirstTime ) {
      openFirstTime = false;
    }
    openFile = true;
  }//open
  /* Close file */
  public void close () {
    try {
      iFile.close();
    } 
    catch ( IOException ex ) {
      System.out.println( "Can't close the file " + inPath );
    }
    openFile = false;
  }//close
  
  /* Give next content.
   * A content is built from a (string) token.
   * Moreove, it holds other useful info related to the
   * token, e.g., line where the token has been found.
   * The token that constitutes the content corresponds
   * to the token read from file according to the internal
   * state of the object (i.e., pointer in the text file).
   * If this function is call and no other
   * content is available it returns nullprt.
   */
  public Content getNextContent () {
    /* Open file if not already open */
    if ( !openFile ) open();
    if ( !openFile ) return null;
    
    /* Check if previous call of the method started to 
       parse a new line. */
    if ( newLine ) {
      currentLine++;
      newLine = false;
    }
    
   /* Check last read: 
    * if already read all the string, 
    * get a new string to tokenize from the last point on.
    */
    if ( token == null ) {
      try {
        iFile.seek( currPos );
      } catch ( IOException ex ) {
        System.out.println("Can't access file " + inPath 
                            +   " at position " + currPos );
        return null;
      }
      /* Check whether there is another line
         * to parse and, if it is the case, get it. */
      try {
        parsedLine = iFile.readLine();
        
        while ( (parsedLine != null) && (parsedLine.length() == 0) ) {
          parsedLine = iFile.readLine();
          if ( parsedLine == null ) {
            moreElements = false;
            close();
            return null;
          }
        }
      } 
      catch ( IOException ex ) {
        parsedLine = "";
      }
      if ( parsedLine != null ) {
        moreElements = true;
        try {
          /* Update position */
          currPos = iFile.getFilePointer();
        } 
        catch ( IOException ex ) {}
        
        /* Init tokenizer with the new line */
        tokenizer = new Tokenization ( parsedLine );
        token     = tokenizer.getToken();
        newLine   = tokenizer.findNewLine();
        
        /* Skip empty strings and return results */
        if ( (token == null) || (token.equals( "" )) ) return null;
        return ( new Token( token, currentLine ) );
      }
      else {
        /* No more line available:
         * close file, and exit.
         */
        moreElements = false;
        close();
        return null;
      }
    }
    /* The line is not completely parsed:
     * get next token.
     */
    token   = tokenizer.getToken();
    newLine = tokenizer.findNewLine();
    /*
     * If the string is terminated:
     * prepare next one.
     * @note: we do not set moreElements to false.
     * This means that the client will try again
     * to check whether there are other lines to read
     * from the file.
     */
    if ( (token == null) || (token.equals( "" )) ) return null;
    
    /* Here everything went well: return the content.
     * @note: skip empty strings
     */
    
    if ( token.equals( "" ) ) return null;
    return ( new Token( token, currentLine ) );
  }//getNextContent

  public Content getNextElement() {
    return getNextContent();
  }//getNextElement
  
}//Parser
