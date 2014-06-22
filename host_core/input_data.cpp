#include "input_data.h"

#include <getopt.h>

using namespace std;


/// Init static variable
Input_data* Input_data::_instance = nullptr;

Input_data::Input_data ( int argc, char* argv[] ) :
_dbg        ( "#log: Input_data - " ),
_verbose    ( 0 ),
_timer      ( 0 ),
_in_file    ( "" ),
_out_file   ( "" ),
_store_type ( 0 ) {
  
  if ( argc < 2 ) {
    print_help();
    exit ( 0 );
  }

  /// Read input
  int c;
  int verbose_flag = 0, time_flag = 0;
  while ( true ) {
    static struct option long_options[] =
    {
      /// These options set a flag
      {"verbose",       no_argument, &_verbose,       1}, /// Print verbose during computation
      {"time",          no_argument, &_timer,         1}, /// Print computational time
      /* 
       * These options don't set a flag.
       * We distinguish them by their indices.
       */
      {"help",          no_argument,       0,       'h'}, /// Print a help message
      {"input",         required_argument, 0,       'i'}, /// Set input file
      {"output",        required_argument, 0,       'o'}, /// Set output file
      {"store",         required_argument, 0,       's'}, /// Select store type
      {0, 0, 0, 0}
    };
    
    /// getopt_long stores the option index here
    int option_index = 0;
    c = getopt_long (argc, argv, "vthi:o:s:",
                     long_options, &option_index);
    /// Detect the end of the options
    if ( c == -1 ) break;
    /// Switch on c to detect the input given by the user
    switch ( c ) {
      case 0:
        /// If this option set a flag, do nothing else now
        if ( long_options[ option_index ].flag != 0 )
          break;
        printf ( "option %s", long_options[ option_index ].name );
        if ( optarg )
          printf (" with arg %s", optarg);
        printf ("\n");
        break;
      case 'h':
        print_help();
        exit( 0 );
      case 'v':
        _verbose = true;
        break;
      case 'i':
        _in_file  = optarg;
        break;
      case 'o':
        _out_file = optarg;
        break;
      case 't':
        _timer = true;
        break;
      case 's':
        _store_type = atoi( optarg );
        break;
      default:
        exit( 0 );
    }//switch
  }//while
  
  /// Exit if the user did not set an input file!
  if ( _in_file == "" ) {
    print_help();
    exit( 0 );
  }
  if ( _verbose   ) puts ("verbose flag is set");
  
  /* Print any remaining command line arguments (not options). */
  if ( optind < argc ) {
    printf ("non-option ARGV-elements: ");
    while ( optind < argc )
      printf ("%s ", argv[ optind++ ]);
    putchar ('\n');
  }
}//Input_data

string
Input_data::get_ifile () const {
  return _in_file;
}//get_ifile

string
Input_data::get_ofile () const {
  return _out_file;
}//get_ifile

bool
Input_data::timer_on () const {
  return _timer;
}//get_ifile

bool
Input_data::verbose () const {
  return _verbose;
}//get_ifile

//Factory_store::DataStoreType
int
Input_data::get_store_type () {
  return _store_type;
}//DataStoreType

void
Input_data::print_help () {
  string spaces = "        ";
  cout << "Usage: ./concordance -i <infile> [options]\n" << endl;
  cout << "          Options           |          Description      \n";
  cout << "=========================== | ===========================\n";
  cout << " -v|--verbose               | - Printf verbose info\n";
  cout << "                            |   during computation.\n";
  cout << " -t|--time                  | - Print computational time.\n";
  cout << " -h|--help                  | - Print this help message.\n";
  cout << " -i|--input      (string)   | - Read input file.\n";
  cout << " -o|--output     (string)   | - Set output file.\n";
  cout << "                            |   Default: text console.\n";
  cout << " -s|--store      (int)      | - Set type of store to use:\n";
  cout << "                            |   0 - Prefix tree.\n";
  cout << "                            |   1 - Red-Black tree.\n";
  cout << "                            |   Default: Prefix tree.\n";
  cout << "=========================== | ===========================\n";
  cout << "You may want to try:\n";
  cout << "\t" << "./concordance -i test/test_input.txt\n";
  cout << "For any questions, feel free to write at: campe8@nmsu.edu.\n";
}//print_help


