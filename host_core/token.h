/**************************************
 * Token:                             *
 * this class represents the concrete *
 * implementation of a content obj    *
 * within a node of the Prefix Tree.  *
 **************************************/
#ifndef CONCORDANCE_TOKEN__
#define CONCORDANCE_TOKEN__

#include "globals.h"
#include "content.h"

class Token : public Content {
private:
  /// String representing the token
  std::string _token;
  /// Lines (from text) where the token has been found
  std::vector <int> _idx_line;
  /// Number of occurences of the token in the text
  int _counter;
  /// Global counter (i.e., num of tokens parsed so far + 1)
  static int _glb_id;
  int _token_id;
public:
  /// For Constructor: token, line, and counter
  Token ();
  Token ( std::string );
  Token ( std::string, int, int=1 );
  Token ( std::string, std::vector <int>, int=1 );
  Token ( const Token& );
  Token& operator= ( const Token& );
  /// Operators for comparing strings
  virtual bool less  ( const Content* ) const;
  virtual bool equal ( const Content* ) const;
  
  ~Token();
  
  /// Get functions
  std::string      get_string  () const;
  std::vector<int> get_lines   () const;
  int              get_counter () const;
  int          get_glb_counter () const;
  int              get_id      () const;
  
  void             set_lines   ( const std::vector<int>& );
  void             set_counter ( int );
  
  /// Check if a content matches somehow the current one
  virtual bool match ( const Content* ) const;
  
  /*
   * Check "how" a given token matches the current one.
   * This is a particular function:
   * it checkes is the match is perfect, a prefix or a suffix.
   * Moreover, it can modify the content given in input but it
   * cannot modify the current content. 
   * If it is the case, it generates a new content.
   */
  virtual void how_match ( Content *, std::vector< Content * >& ) const;
  
  /// Size of the base object: string legth
  virtual size_t get_size () const;
  /// Reference to _token
  virtual const void * get_ref_base_obj () const;
  
   /*
    * Update this content with a new content:
    * if the string of the content
    * given in input is equal to this _token
    * then update the other class fields.
    * Otherwise, replace _token with the string given in input.
   */
  virtual void update_content ( Content * );
  
  /// Overwrite the token with a new one leaving the rest unchanged
  virtual void overwrite_content ( std::string );
  
  /// Print the content of the object
  virtual void print() const;
  /// Print info according to Concordance
  virtual void print_info()      const;
  virtual std::string get_info() const;
};

#endif


