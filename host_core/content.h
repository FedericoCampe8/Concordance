/**************************************
 * Content:                           *
 * this class represents the          *
 * interface of the objects           *
 * stored in each node of the         *
 * Prefix tree.                       *
 * Concrete implementations should    *
 * implement this interface.          *
 **************************************/

#ifndef CONCORDANCE_CONTENT__
#define CONCORDANCE_CONTENT__

#include "globals.h"

class Content {
public:
  virtual ~Content() {};
  
  /*
   * Methods used for comparing contents in the data store.
   * These methods are used for sorting the contents/children.
   */
  virtual bool less  ( const Content* ) const = 0;
  virtual bool equal ( const Content* ) const = 0;
  
  /// Check if a content matches the current one
  virtual bool match ( const Content* ) const = 0;
  
  /*
   * Return the size of the base object representing the content.
   * This is useful to organize or print contents.
   */
  virtual size_t get_size () const = 0;
  
  /*
   * Returns a (void) pointer to the base object of the content:
   * this is useful to extract such object if needed for analyzing it.
   */
  virtual const void * get_ref_base_obj () const = 0;
  
  /*
   * Check "how" a given content matches the current one.
   * It returns a vector of contents:
   * 1 - the vector is empty -> complete match -> perform update current content;
   * 2 - the vector as one entry -> complete match + extension -> add child;
   * 3 - the vector as three entries -> partial match -> split and add.
   * @note: it does not modify the current content if split.
   * the client has to over-write it with the content given back!
   */
  virtual void how_match( Content *, std::vector< Content * >& ) const = 0;
  
  /*
   * Update the current content with a new content 
   * given in input:
   * different derived class may change 
   * how to perform the update.
   */
  virtual void update_content ( Content * ) = 0;

  /// Print the content
  virtual void print() const = 0;
  /*
   * Print info (general).
   * The client can customize this method if needed.
   * @note: the above "print" prints all the info related to 
   * the content, while "print_info" can be customized by the client
   * to print only some information related to the content.
   * @note: get_info is the same as print_info but returns a 
   * string representing the information.
   */
  virtual void        print_info () const {}
  virtual std::string get_info   () const { return ""; }
};

#endif


