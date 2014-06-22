/*****************************************
 * ConcordanceVisitor (concrete Visitor) *
 * this class implements a Visitor for   *
 * visiting the Prefix tree and printing *
 * the Concordance of the parsed text.   *
 **************************************/
#ifndef CONCORDANCE_CON_VISITOR__
#define CONCORDANCE_CON_VISITOR__

#include "globals.h"
#include "visitor.h"
#include "node.h"

class ConcordanceVisitor : public Visitor {
private:
  std::string _out_file;
  std::string _str_to_print;
  std::string _current_token;
  /*
   * Stack used for backtracking on print.
   * It holds: 
   * a) Size of the string
   * b) Level of the tree where the string has been found
   */
  std::stack< std::pair<size_t, int> > _trail_stack;
public:
  ConcordanceVisitor  ( std::string outf="" );
  ~ConcordanceVisitor ();
  
  virtual void visit();
  
  /// Visit methods for nodes
  virtual void visitNode ( Node * );
  
  void print_to_file();
};

#endif


