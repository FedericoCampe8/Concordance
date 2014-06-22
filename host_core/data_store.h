/***************************************
 * Data_store:                         *
 * this class represents an interface  *
 * for the data structure used to      *
 * store the contents (tokens).        *
 * Several data structures can be used *
 * to organize the contents/tokens,    *
 * e.g., prefix tree, buckets, maps.   *
 **************************************/
#ifndef CONCORDANCE_DATA_STORE__
#define CONCORDANCE_DATA_STORE__

#include "globals.h"
#include "input_data.h"
#include "content.h"
#include "parser.h"

class Data_store {
protected:
  /// Options
  bool _verbose;
  bool _timer;
  std::string _out_file;
  /// Variables for options
  timeval _time_stats;
  double  _time_start, _total_time;
  /// Protected Costructor (for singleton instance)
  Data_store () {}
public:
  virtual ~Data_store() {}
  
  /* 
   * Size of the store:
   * different data structure have
   * different definitions of size.
   */
  virtual int get_size () = 0;
  
  /// Add content directly to the store.
  virtual void add_content ( Content* ) = 0;
  
  /*
   * Add content using a Parser.
   * @note: this function is not
   * pure since the client is not
   * required to use a parser to fill the
   * data structure, e.g., useful when
   * testing the code.
   */
  virtual void fill_store ( Parser* ) {}
  
  /* 
   * Print store:
   * it prints the sorted list of the contents.
   */
  virtual void print () = 0;
  
  /*
   * Print the structure of the store.
   * @note: this method is not pure since the user
   * can decide whether or not it is useful to 
   * print the "structure" or other info 
   * about the data store.
   */
  virtual void print_data_store () {}
  
  /*
   * Method for setting some options to the
   * data store (e.g., stdout, verbose, etc.).
   */
  virtual void set_options ( std::string ostr = "",
                             bool timer       = false,
                             bool verbose     = false ) {
    _out_file = ostr;
    _timer    = timer;
    _verbose  = verbose;
  }
  /*
   * Other methods.
   * These methods are used for obtaining some additional info.
   */
  virtual void set_timer () {
    gettimeofday( &_time_stats, NULL );
    _time_start = _time_stats.tv_sec + ( _time_stats.tv_usec / 1000000.0 );
  }//set_timer
  
  virtual double get_timer () {
    gettimeofday( &_time_stats, NULL );
    _total_time = _time_stats.tv_sec + ( _time_stats.tv_usec / 1000000.0 ) - _time_start;
    return _total_time;
  }
};

#endif


