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

package concordance;

/**
 *
 * @author fedecampe
 */
public class Concordance {

  public static void main( String[] args ) {
    
    String dbg = "#log: Main - ";
    /***************************************
     *         INIT DATA/READ INPUT        *
     ***************************************/
    Input_data iData = Input_data.getInstance( args );
    
    /***************************************
     *          CREATE DATA STORE          *
     ***************************************/
      if ( iData.verbose() ) System.out.println( dbg + "Create Data Store\n" );
      AbstractDataStore dStore = (AbstractDataStore) FactoryStore.getStore ( 
              FactoryStore.getStoreType ( iData.getStoreType() ) );
    
    /***************************************
     *           FILL DATA STORE           *
     ***************************************/
      if ( iData.verbose() ) System.out.println( dbg + "Fill Data Store\n" );
      dStore.setOptions(iData.getOFile(), iData.timerOn(), iData.verbose());
      dStore.fillStore( Parser.getInstance( iData.getIFile() ) );
      
    /***************************************
     *       PRINT DATA (CONCORDANCE)      *
     ***************************************/  
      dStore.print ();
  }//main
  
}//Concordance
