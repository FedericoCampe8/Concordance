#include "globals.h"
#include "tokenization.h"

//#define TOKENIZATION_DBG

using namespace std;

Tokenization::Tokenization() :
_c_token     ( nullptr ),
_parsed_line ( nullptr ),
_new_line    ( false ),
_need_line   ( true ) {
}//-

bool
Tokenization::need_line () {
  return _need_line;
}//need_line

void
Tokenization::set_new_tokenizer ( string line ) {
  /// Delete previous allocate memory
  if ( _parsed_line != nullptr ) delete [] _parsed_line;
  /// Allocate memory for the current line
  _parsed_line = new char [ line.length() + 1 ];
  strcpy ( _parsed_line, line.c_str() );
  _need_line = false;
  _c_token = nullptr;
}//set_new_tokenizer

string
Tokenization::get_token() {
  
  /// Init internal state of the tokenizer
  if ( _c_token == nullptr ) {
    _c_token = strtok ( _parsed_line , DELIMITERS.c_str() );
  }
  else {
    /*
     * The line is not completely parsed:
     * get next token.
     */
    _c_token = strtok ( NULL , DELIMITERS.c_str() );
  }
  
  /*
   * If the string is terminated:
   * prepare next one.
   * @note: we do not set _more_elements to false.
   * This means that the client will try again
   * to check whether there are other lines to read
   * from the file.
   */
  if ( _c_token == NULL ) {
    _c_token = nullptr;
    _need_line = true;
    return "";
  }
  
  /*
   * Everything went well: return the content.
   * @note: skip empty strings
   */
  return analyze_token ();
}//get_token

string
Tokenization::analyze_token () {
  
  /*
   * State if current char has to be skipped.
   * For example, skip punctuation.
   */
  bool charToAvoid;
  /// Start from empty string
  string filteredToken = "";
  
  /*
   * Next char is '\0'?
   * Used to skip some chars (e.g., punctuation).
   */
  bool next_char_null = false;
  
  /*
   * Check if we've already parsed at least one char.
   * Used to get prev char and next and decide
   * whether to skip current char or not.
   */
  bool advance  = false;
  
  /*
   * State if current char has to be skipped.
   * For example, skip punctuation.
   */
  bool char_to_avoid;
  /// Start from empty string
  string token = "";
  char token_to_add;
  
#ifdef TOKENIZATION_DBG
  cout << "#log: analyze_token() - input: " <<
  _c_token << endl;
#endif
  
  while ( *(_c_token) ) {
    
    if ( (*(_c_token + 1)) == '\0' ) next_char_null = true;
    /// Check whteher to skip or not the current parsed char
    char_to_avoid = avoid_char ( *_c_token );
    /*
     * Good char if:
     * 1 - it is not a "char to avoid"
     * 2 - it is a "char to avoid" but the previous char
     *     and the next cbar are not "chars to avoid"
     * @note: it accepts tokens like "3.14".
     * @note: it accepts tokens like "e.g.,it"
     * @note: this is a heuristic, it can be changed!
     */
    if ( (!char_to_avoid) ||
         (advance    &&
         (!next_char_null) &&
         ((!avoid_char  ( *(_c_token - 1))) &&
          (!avoid_char  ( *(_c_token + 1)))) ) ) {
           /// Store char (convert uppercase to lowercase)
           token_to_add = *_c_token;
           if ( (token_to_add >= 'A') &&
                (token_to_add <= 'Z') ) {
             token_to_add = token_to_add + ('a' - 'A');
           }
           token += token_to_add;
           
         }
    /*
     * Check whether to set a new line depending on
     * the heuristic used.
     * @note: if false, then the current token is not
     * a new line but it must be inserted as part of the
     * current token. */
    else if ( !set_new_line(_c_token, advance, next_char_null) ) {
      token_to_add = *_c_token;
      token += token_to_add;
    }
    
    /// At least one char has been found
    advance = true;
    _c_token++;
  }//while
  /*
   * Return the string corresponding
   * to the parsed token.
   */
  
#ifdef TOKENIZATION_DBG
  cout << "#log: analyze_token() - return: " <<
  token << endl;
#endif
  
  return token;
}//analyze_token

bool
Tokenization::avoid_char ( char c_val ) {
  /* Skip all symbols
   * but not numbers, lowercase chars,
   * and uppercase chars.
   * These values are taken from the ASCII table.
   */
  return ( (c_val < 48)                    ||
           (c_val > 122)                   ||
           ((c_val > 57) && ( c_val < 65)) ||
           ((c_val > 91) && ( c_val < 97)) );
}//avoid_char

bool
Tokenization::set_new_line ( char * token, bool advance, bool last_char ) {
  if ( (*_c_token) == '.' ) {
    /*
     * Check possible end of the string.
     * New line if:
     * next char is not NULL and it is a capital letter
     * @note: this is a heuristic, it can be changed!
     */
    if ( (!last_char) &&
         ((*(_c_token + 1)) >= 'A') &&
         ((*(_c_token + 1)) <= 'Z') ) {
      _new_line = true;
      return true;
    }
    else if ( last_char ) {
      /*
       * Next char is NULL:
       * we are at the end of a line ->
       * next time this method is invoked it increases
       * the number of parsed lines.
       */
      _new_line = true;
      return true;
    }
    else if ( advance         &&
             (!last_char)     &&
             ((!avoid_char ( *(_c_token - 1))) ||
             (!avoid_char  ( *(_c_token + 1)))) ) {
               /*
                * Particular cases and heuristics:
                * the symbol "." is within a token like "e.g.,"
                */
               _new_line = false;
               return false;
             }
  }
  return true;
    
}//set_new_line

bool
Tokenization::find_new_line () {
  if ( _new_line ) {
    _new_line = false;
    return true;
  }
  return false;
}//find_new_line


