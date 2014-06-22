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
#include "prefix_tree.h"
#include "input_data.h"
#include "data_store.h"
#include "factory_store.h"
#include "parser.h"

#include "content.h"
#include "token.h"

using namespace std;

//#define TEST_AUX_1
//#define TEST_AUX_2

void test_aux();

int main ( int argc, char* argv[] ) {
  
  if ( (argc > 1) && atoi ( argv[ 1 ] ) ) {
    test_aux();
    return 0;
  }
  
  const int MAX_LINE_SIZE = 256;
  char line [ 256 ];
  int text_line = 0, output_selection = 0;
  string str_line;
  
  /// Create a data store
  Data_store * dt_store = Factory_store::get_store ( Factory_store::PrefixTree );
  /// Define root token...
  Token * root_tk = new Token ( "" );
  /// ...add root token to the tree
  dt_store->add_content( root_tk );
  
  cout << "Print the Data Store (default: No)? (Y/N)\n";
  cin.getline ( line, 256 );
  str_line = line;
  output_selection = (!str_line.compare ("Y"))   ||
                     (!str_line.compare ("y"))   ||
                     (!str_line.compare ("yes")) ||
                     (!str_line.compare ("Yes")) ||
                     (!str_line.compare ("YES"));
  
  cout << "Insert a word (type \"exit\" to stop the process):\n";
  cin.getline ( line, 256 );
  str_line = line;
  while ( str_line.compare ( "exit" ) ) {
    /// Add word to the tree
    cout << "Token to insert:\t" << str_line << endl;
    dt_store->add_content( new Token ( str_line, text_line++ ) );
    /// Print the concordance tree (at each insertion)
    if ( output_selection ) {
      cout << "------- Data Store --------\n";
      dt_store->print_data_store();
      cout << "---------------------------\n";
    }
    cout << "------- Concordance -------\n";
    dt_store->print();
    cout << "---------------------------\n";
    cin.getline ( line, 256 );
    str_line = line;
  }//while
  
  /*
   * Clear and exit.
   * @note: everything is cleaned by
   * the deleting the data store.
   */
  delete dt_store;
  
  return 0;
}//main


void test_aux () {
  
  /// Create a data store
  Data_store * d_store = Factory_store::get_store ( Factory_store::PrefixTree );
  
  /// Define some tokens...
  Token * tk_a = new Token ( ""         );
#ifdef TEST_AUX_1
  Token * tk_b = new Token ( "acdc",        1 );
  Token * tk_c = new Token ( "zztop",       1 );
  Token * tk_d = new Token ( "queen",       1 );
  Token * tk_e = new Token ( "metallica",   2 );
  Token * tk_f = new Token ( "LedZeppelin", 3 );
  Token * tk_g = new Token ( "PinkFloyd",   3 );
#elif TEST_AUX_2
  Token * tk_b = new Token ( "page",        1 );
  Token * tk_c = new Token ( "present",     1 );
  Token * tk_d = new Token ( "printed",     2 );
  Token * tk_e = new Token ( "printed",     3 );
  Token * tk_f = new Token ( "program",     2 );
  Token * tk_g = new Token ( "programming", 2 );
#else
  Token * tk_b = new Token ( "a",            1 );
  Token * tk_c = new Token ( "algorithm",    1 );
  Token * tk_d = new Token ( "a",            2 );
  Token * tk_e = new Token ( "an",           3 );
  Token * tk_f = new Token ( "alphabetical", 2 );
  Token * tk_g = new Token ( "all",          2 );
#endif
  
  /// ...add these tokens to the tree
  d_store->add_content( tk_a );
  d_store->add_content( tk_b );
  d_store->add_content( tk_c );
  d_store->add_content( tk_d );
  d_store->add_content( tk_e );
  d_store->add_content( tk_g );
  d_store->add_content( tk_f );
  
  /// Print the resulting tree
  cout << "Data store:\n";
  d_store->print_data_store();
  /// Print the resulting concordance
  cout << "Concordance:\n";
  d_store->print();
  
  /*
   * Clear and exit.
   * @note: everything is cleaned by
   * the deleting the data store.
   */
  delete d_store;
}//test_aux

