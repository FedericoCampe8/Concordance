/***************************************
 * Factory_store:                      *
 * this class is used only to allow    *
 * the user to select different kinds  *
 * of store to use .                   *
 * It creates a singleton instance of  *
 * the store.                          *
 **************************************/
#ifndef CONCORDANCE_FACTORY_STORE__
#define CONCORDANCE_FACTORY_STORE__

#include "globals.h"
#include "data_store.h"
#include "prefix_tree.h"
#include "rb_tree.h"

class Factory_store {
protected:
  Factory_store () {}
private:
  static Data_store* _ds_instance;
public:
  enum DataStoreType {
    PrefixTree,
    RBTree
    //Buckets
  };
  
  /// Wrapper from int to DataStoreType
  static DataStoreType get_store_type ( int s_type ) {
    switch ( s_type ) {
      case 0:
        return DataStoreType::PrefixTree;
      case 1:
        return DataStoreType::RBTree;
      default:
        return DataStoreType::PrefixTree;
    }
  }//get_type
  
  /// Get the unique instance (static) of singleton
  static Data_store* get_store( DataStoreType ds_type ) {
    if ( _ds_instance == nullptr ) {
      switch ( ds_type ) {
        case PrefixTree:
          _ds_instance =  new Prefix_tree();
          break;
        case RBTree:
          _ds_instance =  new RB_Tree();
          break;
        default:
          _ds_instance =  new Prefix_tree();
          break;
      }
    }
    return _ds_instance;
  }//get_store
  
};

#endif


