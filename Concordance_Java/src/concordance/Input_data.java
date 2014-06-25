/****************************************************
 * Input data class:                                *
 * Singleton class used for reading                 *
 * input options given by the user.                 *
 ****************************************************/

package concordance;

class Input_data {
  /* Debug string */
  private String dbg;
  /* Static instance for singleton obj */
  protected static Input_data instance = null;
  /* General info */
  private boolean verbose;
  private boolean timer;
  private int     storeType;
  private String  iFile;
  private String  oFile;
  
  private void printHelp() {
    String helpString = 
              "          Options           |          Description      \n"
            + "=========================== | ===========================\n"
            + " -v|--verbose               | - Printf verbose info\n"
            + "                            |   during computation.\n"
            + " -t|--time                  | - Print computational time.\n"
            + " -h|--help                  | - Print this help message.\n"
            + " -i|--input      (string)   | - Read input file.\n"
            + " -o|--output     (string)   | - Set output file.\n"
            + "                            |   Default: text console.\n"
            + " -s|--store      (int)      | - Set type of store to use:\n"
            + "                            |   0 - Prefix tree.\n"
            + "                            |   1 - Red-Black tree.\n"
            + "                            |   Default: Prefix tree.\n"
            + "=========================== | ===========================\n"
            + "You may want to try:\n"
            + "\tjava -jar Concordance.jar -i test/test_input.txt\n"
            + "For any questions, feel free to write at: campe8@nmsu.edu.\n";
    System.out.println( "Usage: java -jar Concordance.jar -i <infile> [options]" );
    System.out.println( helpString );
  }//printHelp
  
  /* Protected constructor: a client that tries to instantiate
   * Singleton directly will get an error.*/ 
  protected Input_data( String[] args ) {
    init();
    if ( args.length < 1 ) {
      printHelp();
      System.exit( 0 );
    }
    else {
      /* Parsing the inputs... */
      for ( int i = 0; i < args.length; i++ ) {
        if ( args[ i ].contentEquals( "--verbose" ) || 
             args[ i ].contentEquals( "-v" ) ) {
          verbose = true; 
          continue;
        }
        if ( args[ i ].contentEquals( "--time" ) || 
             args[ i ].contentEquals( "-t" ) ) {
          timer = true; 
          continue;
        }
        if ( args[ i ].contentEquals( "--input" ) || 
             args[ i ].contentEquals( "-i" ) ) {
          if ( (i+1) == args.length ) break;
          iFile = args[ i + 1 ]; 
          continue;
        }
        if ( args[ i ].contentEquals( "--output" ) || 
             args[ i ].contentEquals( "-o" ) ) {
          if ( (i+1) == args.length ) break;
          oFile = args[ i + 1 ]; 
          continue;
        }
        if ( args[ i ].contentEquals( "--store" ) || 
             args[ i ].contentEquals( "-s" ) ) {
          if ( (i+1) == args.length ) break;
          try {
            storeType = Integer.parseInt( args[ i + 1 ] );
          } catch ( NumberFormatException nfe ) { storeType = 0; }
          continue;
        }
        if ( args[ i ].contentEquals( "--help" ) || 
             args[ i ].contentEquals( "-h" ) ) {
          printHelp();
          System.exit( 0 );
        }
      }
    }
  }//Input_data
  
  private void init() {
    dbg = "#log: Input_data - ";
    verbose   = false;
    timer     = false;
    iFile     = "";
    oFile     = "";
    storeType = 0;
  }//init
  
  /* Constructor:
   * Get the unique instance (static) of singleton object. */
  public static Input_data getInstance( String[] args ) {
    if ( instance == null ) {
      instance = new Input_data( args );
    }
    return instance;
  }//getInstance
  
  /* Get Methods */
  public String  getIFile() { return iFile;     }
  public String  getOFile() { return oFile;     }
  public boolean timerOn()  { return timer;     }
  public boolean verbose()  { return verbose;   }
  public int getStoreType() { return storeType; }
}//Input_data
