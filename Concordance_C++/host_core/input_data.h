/****************************************************
 * Input data class:                                *
 * Singleton class used for reading                 *
 * input options given by the user.                 *
 ****************************************************/

#ifndef CONCORDANCE_INPUT_DATA__
#define CONCORDANCE_INPUT_DATA__

#include "globals.h"

class Input_data {
private:
  /// Debug string
  std::string _dbg;
  /// Static instance for singleton obj
  static Input_data* _instance;
  /// Other
  int _timer;
  int _verbose;
  int _store_type;
  std::string _in_file;
  std::string _out_file;
  void print_help ();
protected:
  /*
   * Protected constructor: a client that tries to instantiate
   * Singleton directly will get an error at compile-time.
   */
  Input_data ( int argc, char* argv[] );
public:
  /*
   * Constructor:
   * Get the unique instance (static) of singleton.
   */
  static Input_data* get_instance( int argc, char* argv[] ) {
    if ( _instance == nullptr ) {
      _instance = new Input_data ( argc, argv );
    }
    return _instance;
  }//get_instance
  
  /// Get methods
  std::string get_ifile() const;
  std::string get_ofile() const;
  bool timer_on() const;
  bool verbose() const;
  int get_store_type ();
};

#endif
