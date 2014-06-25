/***************************************
 * AbstractDatastore:                  *
 * this class represents an abstract   *
 * class for a "general" data structure*
 * that can be used as data store.     *
 **************************************/
package concordance;

abstract class AbstractDataStore implements DataStore {
  /* Private members, just needed in this abstract class */
  protected long startTimeMs;
  protected boolean timer;
  protected boolean verbose;
  protected String outFile;
  
  /* Protected constructore: init values */
  protected AbstractDataStore () {
    timer   = false;
    verbose = false;
    outFile = "";
  }//AbstractDataStore
  
  /* Add content using a Parser. */
  abstract void fillStore( Parser prs );
  
  /* Public methods, avaialable to the client */
  public void setTime() {
    startTimeMs = System.currentTimeMillis();
  }//setTime
  
  public long getTime() {
    return ( System.currentTimeMillis() - startTimeMs );
  }//getTime
  
  /* Set options from user input */
  public void setOptions ( String oFile, boolean timer, boolean verbose ) {
    outFile = oFile;
    this.timer = timer;
    this.verbose = verbose;
  }//setOptions
  
  /* Print the structure of the store.
   * @note: the client can decide whether 
   * or not it is useful to 
   * print the "structure" or other info 
   * about the data store.
   */
  public void print_data_store () {}
  
}//AbstractDataStore
