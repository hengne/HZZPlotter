
// Auther: Hengne.Li@cern.ch   
//         University of Virginia, 2014

#include <stdio.h>
#include <iostream>
#include <string>
#include "config.hpp"

int main(int argc, char* argv[])
{
  if( argc<2 )
  {
    std::cout << argv[0] << " config_file " << std::endl;
    return 0;
  }

  // open steering file names
  config steer(argv[1]);
  //mode = steer.getInt("mode");

  /////////
  // close fout
  //fout->Close();
  return 0;

}

