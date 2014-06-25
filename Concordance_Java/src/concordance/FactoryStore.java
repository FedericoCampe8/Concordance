/***************************************
 * Factory_store:                      *
 * this class is used only to allow    *
 * the user to select different kinds  *
 * of store to use .                   *
 * It creates a singleton instance of  *
 * the store.                          *
 **************************************/
package concordance;


public class FactoryStore {
  private static DataStore instance = null;
  
  protected FactoryStore () {}
  
  public enum DataStoreType {
    PREFIXTREE,
    RBTREE
  };
  
  public static DataStoreType getStoreType ( int storeType ) {
    switch ( storeType ) {
      case 0:
        return DataStoreType.PREFIXTREE;
      case 1:
        return DataStoreType.RBTREE;
      default:
        return DataStoreType.PREFIXTREE;
    }
  }//getStoreType
  
  /* Get the unique instance (static) of singleton */
  public static DataStore getStore ( DataStoreType dst ) {
    if ( instance == null ){
      switch ( dst ) {
        case PREFIXTREE:
          instance = new PrefixTree();
          break;
        case RBTREE:
          instance = new RBTree();
          break;
        default:
          instance = new PrefixTree();
          break;
      }
    }
    return instance;
  }//getStore
  
}//FactoryStore
