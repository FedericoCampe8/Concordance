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
  _delimiters = " \t\r\n";
  tokenizer   = new Tokenization();
}//-

void
Parser::add_delimiter ( string del ) {
  _delimiters += del;
}//add_delimiter

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
}//get_next_token

int
Parser::get_current_line () {
  return _current_line;
}//get_line

Content *
Parser::get_next_content () {
  /// Open stream (if not already opened)
  if ( !_open_file ) { open(); }
  
  /* 
   * Check if previous call of the method started to
   * parse a new line. 
   */
  if ( _new_line ) {
    _current_line++;
    _new_line = false;
  }
  
  /*
   * Check last read: 
   * if already read all the string, 
   * get a new string to tokenize.
   */
  if ( tokenizer->need_line() ) {
    /// Set position on file to the most recent position
    _if_stream->seekg ( _curr_pos );
    string line;
    /*
     * Check whether there is another line
     * to parse and, if it is the case, get it.
     */
    if ( getline ( *_if_stream, line ) ) {
      while ( line.size() == 0 ) {
        if ( !getline ( *_if_stream, line ) ) {
          /*
           * No more line available:
           * close file, clear everything and exit.
           */
          _more_elements = false;
          close();
          return nullptr;
        }
      }
      _more_elements = true;
      
      /// Update position
      _curr_pos = _if_stream->tellg();
      
      /// Get token
      tokenizer->set_new_tokenizer( line );
      _token     = tokenizer->get_token();
      _new_line  = tokenizer->find_new_line();
      
      if ( _token.compare( "" ) == 0 ) { return nullptr; }
      return ( new Token( _token, _current_line ) );
    }
    else {
      /*
       * No more line available:
       * close file, clear everything and exit.
       */
      _more_elements = false;
      close();
      return nullptr;
    }
  }

  /*
   * Everything went well: return the content.
   * @note: skip empty strings
   */
  _token     = tokenizer->get_token();
  _new_line  = tokenizer->find_new_line();
  
  if ( _token.compare( "" ) == 0 ) { return nullptr; }
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
