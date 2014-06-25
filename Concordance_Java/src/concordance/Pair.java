/***************************
 * Utility class for pairs *
 ***************************/
package concordance;

public class Pair {
  private int tokenSize;
  private int level;
  
  /* Pairs used in the Stack.
   * Stack is used for backtracking on print.
   * It holds: 
   * a) Size of the string
   * b) Level of the tree where the string has been found
   */
  public Pair ( int tokenSize, int level ) {
    this.tokenSize = tokenSize;
    this.level     = level;
  }//Pair
  
  public int  first() {
    return tokenSize;
  }//first
  
  public int second() {
    return level;
  }//second
}//Pair
