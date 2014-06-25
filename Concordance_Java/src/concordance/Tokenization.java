/****************************************************
 * Tokenization class extends StringTokenizer.      *
 * Some options are customized inside here.         *
 ****************************************************/
package concordance;

import java.util.StringTokenizer;

public class Tokenization extends StringTokenizer {
  private static final String DELIMITERS = "\t\r\n ";
  private static final String WHITESPACE = " \t\r\n";
  /* Variables for identifying when a new line has been found */
  private boolean newLine;
  
  private boolean avoidChar( char charAt ) {
    /* Skip all symbols
     * but not numbers, lowercase chars,
     * and uppercase chars.
     * These values are taken from the ASCII table. */
  return ( (charAt < 48)                     ||
           (charAt > 122)                    ||
           ((charAt > 57) && ( charAt < 65)) ||
           ((charAt > 91) && ( charAt < 97)) );
  }//avoidChar
  
  /* Changes the internal state:
   * new line has been found.
   * newLine will be true for next token.
   * @note: here one can chage the definition about
   * what a new line actually means. */
  private boolean setNewLine( String token, int idx ) {
    
    if ( token.charAt(idx) == '.' ) {
        /* Check possible end of the string.
         * New line if:
         * next char is not NULL and it is a capital letter
         * @note: this is a heuristic, it can be changed! */
        if ( (idx < (token.length() - 1)) &&
             ((token.charAt(idx+1) >= 'A')        &&
              (token.charAt(idx+1) <= 'Z')  ) ) {
          newLine = true;
          return true;
        }
        else if (idx == (token.length() - 1)) {
           /* Next char is NULL:
            * we are at the end of a line ->
            * next time this method is invoked it increases
            * the number of parsed lines.
            */
          newLine = true;
          return true;
        }
        else if ( (idx > 0)                    &&
                  (idx < (token.length() - 1)) &&
                  ((!avoidChar ( token.charAt(idx-1))) ||
                  ( !avoidChar ( token.charAt(idx+1) )))) {
          /*
           * Particular cases and heuristics:
           * the symbol "." is within a token like "e.g.," */
           //filteredToken += token.charAt(i);
          newLine = false;
          return false;
        }
      }
    
    return true;
  }//setNewLine
  
  
  public Tokenization ( String stm ) {
    super ( stm, DELIMITERS, true );
    newLine  = false;
  }//Tokenization
  public Tokenization ( String stm, String newDelims ) {
    super ( stm, DELIMITERS + newDelims, true );
    newLine  = false;
  }//Tokenization
  
  
  
  @Override
  public String nextToken()  {
        String token = null;
        if ( !hasMoreTokens() ) return token;
        do {
            token = super.nextToken();
        } while ( (WHITESPACE.indexOf( token ) >= 0) && hasMoreTokens() );
        return token;
  }//nextToken
  
  /* Analyze token:
   * this function acts like a filter.
   * It analyzes _c_token and returns
   * a string corresponding to the token cleaned from
   * useless chars. */
  public String getToken () {
    String token = nextToken();
    if ( token == null ) return null;
    /* State if current char has to be skipped.
     * For example, skip punctuation. */
    boolean charToAvoid;
    /* Start from empty string */
    String filteredToken = "";

    for ( int i = 0; i < token.length(); i++ ) {
      charToAvoid = avoidChar ( token.charAt(i) );
      /* Good char if: 
       * 1 - it is not a "char to avoid"
       * 2 - it is a "char to avoid" but the previous char
       *     and the next cbar are not "chars to avoid"
       * @note: it accepts tokens like "3.14".
       * @note: it accepts tokens like "e.g.,it"
       * @note: this is a heuristic, it can be changed!
       */
      if ( (!charToAvoid) ||
           ((i>0)    &&
           (i < (token.length() - 1) )  &&
           ((!avoidChar ( token.charAt(i-1))) &&
           ( !avoidChar ( token.charAt(i+1)))) ) ) {
            /// Store char (convert uppercase to lowercase)
            char tokenToAdd = token.charAt(i);
            if ( (tokenToAdd >= 'A') &&
                 (tokenToAdd <= 'Z') ) {
              tokenToAdd = (char) ((int)tokenToAdd + (int)('a' - 'A'));
            }
            filteredToken += tokenToAdd;
      }
      
      /* Check whether to set a new line depending on
       * the heuristic used.
       * @note: if false, then the current token is not 
       * a new line but it must be inserted as part of the 
       * current token. */
      else if ( !setNewLine(token, i) ) {
        filteredToken += token.charAt(i);
      }
    }//i
              
    /* Return the string corresponding
     * to the parsed token. */
    return filteredToken;
  }//getToken
  
  /* Ask whether a new line has been (previously) found. */
  public boolean findNewLine() {
    if ( newLine ) {
      newLine = false;
      return true;
    }
    return false;
  }//findNewLine
  
}//Tokeniation
