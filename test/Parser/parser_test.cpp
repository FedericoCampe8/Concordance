/*********************************************************************
 * Authors: Federico Campeotto (campe8@nmsu.edu)                     *
 *                                                                   *
 * (C) Copyright 2014                                                *
 *                                                                   *
 * This file is part of Concordance                                  *
 * for protein Structure prediction).                                *
 *                                                                   *
 * Concordance is free software; you can redistribute it and/or      *
 * modify it under the terms of the GNU General Public License       *
 * as published by the Free Software Foundation;                     *
 *                                                                   *
 * Concordance is distributed WITHOUT ANY WARRANTY; without even the *
 * implied  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  *
 * PURPOSE. See the GNU General Public License for more details.     *
 *                                                                   *
 * You should have received a copy of the GNU General Public License *
 * along with this program; if not, see http://www.gnu.org/licenses. *
 *                                                                   *
 *********************************************************************/
#include "globals.h"
#include "parser.h"

using namespace std;

int main ( int argc, char* argv[] ) {
  string dbg = "#log: Main - ";
  
  if ( argc < 2 ) {
    cout << "Usage:\n";
    cout << "./parser <input_file>\n";
    exit( 0 );
  }
  
  string path = argv[1];
  Parser * parser = Parser::get_instance ( path );
  
  string parsed_token;
  while ( parser->has_more_elements() ) {
    parsed_token = parser->get_next_token ();
    if ( parsed_token.compare( "" ) == 0) continue;
    cout << parsed_token << "\t" <<
    " at line: " << parser->get_current_line() << endl;
  }
  
  return 0;
}//main
