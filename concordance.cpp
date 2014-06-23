/*********************************************************************
 * Authors: Federico Campeotto (campe8@nmsu.edu)                     *
 *                                                                   *
 * (C) Copyright 2014                                                *
 *                                                                   *
 * This file is part of Concordance.                                 *
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
#include "input_data.h"
#include "data_store.h"
#include "factory_store.h"
#include "parser.h"

int main ( int argc, char* argv[] ) {
  std::string dbg = "#log: Main - ";

  /***************************************
   *         INIT DATA/READ INPUT        *
   ***************************************/
  Input_data * i_data = Input_data::get_instance ( argc, argv );
  
  /***************************************
   *          CREATE DATA STORE          *
   ***************************************/
  if ( i_data->verbose() ) std::cout << dbg << "Create Data Store\n";
  Data_store * d_store = Factory_store::get_store ( Factory_store::get_store_type ( i_data->get_store_type() ) );

  /***************************************
   *           FILL DATA STORE           *
   ***************************************/
  if ( i_data->verbose() ) std::cout << dbg << "Fill Data Store\n";
  d_store->set_options ( i_data->get_ofile (),
                         i_data->timer_on  (),
                         i_data->verbose   () );
  d_store->fill_store  ( Parser::get_instance ( i_data->get_ifile() ) );
  
  /***************************************
   *       PRINT DATA (CONCORDANCE)      *
   ***************************************/
  if ( i_data->verbose() ) std::cout << dbg << "Print Concordance\n";
  d_store->print ();

  /***************************************
   *           CLEAR AND EXIT            *
   ***************************************/
  if ( i_data->verbose() ) std::cout << dbg << "Clear data and exit\n";
  delete d_store;
  delete i_data;

  return 0;
}//main
