/*****************************************************
 * Tokenization class implements a tokenizer.        *
 * It implements the heuristics used to tokenize the *
 * string read by the parser.                        *
 *****************************************************/

#ifndef CONCORDANCE_TOKENIZATION__
#define CONCORDANCE_TOKENIZATION__

#include "globals.h"
#include "content.h"

class Tokenization {
private:
  const std::string DELIMITERS = "\t\r\n ";
  const std::string WHITESPACE = " \t\r\n";
  /// Variables for identifying when a new line has been found
  bool _new_line;
  bool _need_line;
  /// Token returned by strtok
  char * _c_token;
  /// Parsed line
  char * _parsed_line;
  
  /// Other useful methods to tokenize a string
  /// It states whether the current char has to be skipped or not
  bool avoid_char ( char charAt );
  /* 
   * Checks whether there is a new line in the text.
   * Different heuristics may be used here.
   */
  bool set_new_line ( char * token, bool advance, bool last_char );
  /*
   * Analyze token:
   * this function acts like a filter.
   * It analyzes _c_token and returns
   * a string corresponding to the token cleaned from
   * useless chars. */
  std::string analyze_token ();
public:

  Tokenization ();
  
  /// Prepare a new tokenizer (i.e., string for strtok)
  void set_new_tokenizer ( std::string );
  /// Get the string correspondent to the (filtered) token
  std::string get_token ();
  /// Informs whether a new line has been found
  bool find_new_line ();
  /// Asks whether the tokinizer has finished all the tokens
  bool need_line ();
}; //input_data

#endif
