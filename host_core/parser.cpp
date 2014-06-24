#include "parser.h"
#include "token.h"

//#define PARSER_DBG

using namespace std;

/// Init static variable
Parser* Parser::_instance = nullptr;

Parser::Parser   ( string ifile ) :
_input_path      ( ifile ),
_open_file       ( false ),
_open_first_time ( true ),
_more_elements   ( true ),
_new_line        ( true ),
_current_line    ( -1 ),
_token           ( "" ) {
  _c_token     = nullptr;
  _parsed_line = nullptr;
  _delimiters = " \t\r\n";
}//-

void
Parser::add_delimiter ( string del ) {
  _delimiters += del;
}//add_delimiter

string
Parser::analyze_token () {
   /// Check if started to parse a new line.
  if ( _new_line ) {
    _current_line++;
    _new_line = false;
  }
  /// Proceed in parsing the array of chars
#ifdef PARSER_DBG
  cout << "#log: analyze_token() - char* : " <<
  _c_token << endl;
#endif
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
  _token = "";
  char token_to_add;
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
          ((!avoid_char ( *(_c_token - 1))) &&
           (!avoid_char  ( *(_c_token + 1)))) ) ) {
            /// Store char (convert uppercase to lowercase)
            token_to_add = *_c_token;
            if ( (token_to_add >= 'A') &&
                 (token_to_add <= 'Z') ) {
              token_to_add = token_to_add + ('a' - 'A');
            }
            _token += token_to_add;
    }
    else if ( (*_c_token) == '.' ) {
      /* 
       * Check possible end of the string.
       * New line if:
       * next char is not NULL and it is a capital letter
       * @note: this is a heuristic, it can be changed!
       */
      if ( (!next_char_null) &&
           ((*(_c_token + 1)) >= 'A') &&
           ((*(_c_token + 1)) <= 'Z') ) {
        _new_line = true;
      }
      else if ( next_char_null ) {
        /*
         * Next char is NULL:
         * we are at the end of a line ->
         * next time this method is invoked it increases
         * the number of parsed lines.
         */
        _new_line = true;
      }
      else if ( advance             &&
              (!next_char_null)     &&
               ((!avoid_char ( *(_c_token - 1))) ||
                (!avoid_char  ( *(_c_token + 1)))) ) {
                 /*
                  * Particular cases and heuristics:
                  * the symbol "." is within a token like "e.g.,"
                  */
                 token_to_add = *_c_token;
                 _token += token_to_add;
      }
    }
    /// At least one char has been found
    advance = true;
    _c_token++;
  }//while
  /*
   * Return the string corresponding
   * to the parsed token.
   */
#ifdef PARSER_DBG
  cout << "#log: analyze_token() - return: " <<
  _token << endl;
#endif
  return _token;
}//analyze_token

bool
Parser::avoid_char ( char c_val ) {
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
Parser::has_more_elements () {
  return _more_elements;
}//has_more_elements

string
Parser::get_next_token () {
  Content * content = get_next_content();
  if ( _more_elements && (content != nullptr) ) {
    _token = *((string *) content->get_ref_base_obj());
  }
  else {
    _token = "";
  }
  return _token;
}//get_next_element

int
Parser::get_current_line () {
  return _current_line;
}//get_line

Content *
Parser::get_next_content () {
  /// Open stream (if not already opened)
  if ( !_open_file ) { open(); }
  
  /*
   * Check last read: 
   * if already read all the string, 
   * get a new string to tokenize.
   */
  if ( _c_token == nullptr ) {

    /// Set position on file to the most recent position
    _if_stream->seekg ( _curr_pos );
    string line;
    /*
     * Check whether there is another line
     * to parse and, if it is the case, get it.
     */
    if ( getline ( *_if_stream, line ) ) {
      while ( line.size() == 0 ) {
        getline ( *_if_stream, line );
      }
      _more_elements = true;
      /// Update position
      _curr_pos = _if_stream->tellg();
      
     /// Delete previous allocate memory
      if ( _parsed_line != nullptr ) {
        delete [] _parsed_line;
      }
      /// Allocate memory for the current line
      _parsed_line = new char [ line.length() + 1 ];
      strcpy ( _parsed_line, line.c_str() );
       /// Init internal state of the tokenizer
      _c_token = strtok ( _parsed_line , _delimiters.c_str() );
      _token = analyze_token();
      
      /// Skip empty strings and return results
      if ( _token.compare( "" ) == 0) return nullptr;
      return ( new Token( _token, _current_line ) );
    }
    else {
      /*
       * No more line available:
       * close file, clear everything and exit.
       */
      _more_elements = false;
      if ( _c_token     != nullptr ) free ( _c_token );
      if ( _parsed_line != nullptr ) delete [] _parsed_line;
      close();
      return nullptr;
    }
  }

  /*
   * The line is not completely parsed:
   * get next token.
   */
  _c_token = strtok ( NULL , _delimiters.c_str() );
  
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
    return nullptr;
  }
  
  /*
   * Everything went well: return the content.
   * @note: skip empty strings
   */
  _token = analyze_token();
  if ( _token.compare( "" ) == 0) return nullptr;
  return ( new Token( _token, _current_line ) );
}//get_next_content

void
Parser::open () {
  if ( !_open_file ) {
    _if_stream = new ifstream( _input_path, ifstream::in );
    if ( !_if_stream->is_open() ) {
      /*
       * Some problems here. 
       * @note: we set more elements to false instead of 
       * exiting the program.
       * For example, it may happen the case where the file
       * is moved between a series of open() - close() calls.
       * The client wants to preserve the part of the file
       * already read.
       */
      cout << "Can't open the file: " << _input_path << endl;
      _more_elements = false;
      return;
    }
    if ( _open_first_time ) {
      _curr_pos  = _if_stream->beg;
      _open_first_time  = false;
    }
    _open_file = true;
  }
}//open

void
Parser::close () {
  _if_stream->close();
  _open_file = false;
}//close
