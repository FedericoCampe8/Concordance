/*****************************************
 * Data_store:                           *
 * this class represents an interface    *
 * for the data structure used to        *
 * store contents.                       *
 * Several data structures can be used   *
 * to organize the contents/tokens,      *
 * e.g., prefix tree, buckets, maps, etc.*   
 * but any store must implement at least *
 * these operations described in the     *
 * interface.                            *
 *****************************************/

package concordance;

interface DataStore {
  /* 
   * Size of the store:
   * different data structure have different 
   * definitions of "size".
   */
  int getSize();
  
  /* Add content directly to the store. */
  void addContent( Content cnt );
  
  /* Print store:
   * it prints the sorted list of the contents.
   */
  void print();
}//DataStore
