/*********************************************************
 * Parser class:                                         *
 * Singleton class used for parsing the input file       *
 * given by the user.                                    *
 * It has methods to tokenize a text and give them       *
 * back to the client.                                   *
 * @todo: implement as an observer where the subject     *
 * is the tokenizer that asks/requires lines to tokenize *
 *********************************************************/

#ifndef CONCORDANCE_PARSER_
#define CONCORDANCE_PARSER_

#include "globals.h"
#include "content.h"
#include "tokenization.h"

class Parser {
private:
  /// Static instance for singleton obj
  static Parser* _instance;
  /// Tokenizer
  Tokenization* tokenizer;
  /// Input stream (from file)
  std::ifstream  * _if_stream;
  std::string _input_path;
  bool _open_file;
  bool _open_first_time;
  bool _more_elements;
  bool _new_line;
  /// String token to return
  std::string _token;
  /// "Semantic" number of lines read so far
  int _current_line;
  /// Delimiter to use to tokenize words
  std::string _delimiters;
  /// Other variables needed to move into the file
  std::streampos _curr_pos;
  
protected:
  /*
   * Protected constructor: a client that tries to instantiate
   * Singleton directly will get an error at compile-time.
   */
  Parser ( std::string );
public:
  /*
   * Constructor:
   * Get the unique instance (static) of singleton.
   */
  static Parser* get_instance( std::string ifile ) {
    if ( _instance == nullptr ) {
      _instance = new Parser ( ifile );
    }
    return _instance;
  }//get_idata_instance
  
  /*
   * Add delimiter to tokenizer.
   * @todo: add possibility to add
   * delimiters from command line.
   */
  void add_delimiter ( std::string );
  
  /*
   * Check if the internal status has more tokens to
   * give back to the client.
   */
  bool has_more_elements ();
  
  /*
   * Give next token.
   * This function returns a string corresponding
   * to the token parsed according to the internal 
   * state of the object (i.e., pointer in the text file).
   */
  std::string get_next_token ();
  
  /// Get current line
  int get_current_line ();
  
  /*
   * Give next content.
   * A content is built from a (string) token.
   * Moreove, it holds other useful info related to the
   * token, e.g., line where the token has been found.
   * The token that constitutes the content corresponds
   * to the token read from file according to the internal
   * state of the object (i.e., pointer in the text file).
   * If this function is call and no other
   * content is available it returns nullprt.
   */
  Content * get_next_content ();
  
  /*
   * Open the file.
   * The file is open (if not already open) and the pointer is placed
   * on the last position read.
   * If the file is open for the first time,
   * the pointer is placed on the first position.
   */
  void open ();
  
  /*
   * Close the file.
   * @note: alternating open() and close() 
   * the client can decided how much text
   * has to be parsed.
   */
  void close ();
}; //input_data

#endif
