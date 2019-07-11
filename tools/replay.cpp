//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016-19, Lawrence Livermore National Security, LLC and Umpire
// project contributors. See the COPYRIGHT file for details.
//
// SPDX-License-Identifier: (MIT)
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <ctime>
#include <ratio>
#include <chrono>

#include "umpire/tpl/cxxopts/include/cxxopts.hpp"
#include "util/Replay.hpp"

int main(int argc, char* argv[])
{
  cxxopts::Options options(argv[0], "Replay an umpire session from a file");

  options
    .add_options()
    (  "h, help"
     , "Print help"
    )
    (  "t, time"
     , "Display replay times"
    )
    (  "i, infile"
     , "Input file created by Umpire library with UMPIRE_REPLAY=On"
     , cxxopts::value<std::string>(), "FILE"
    )
  ;

  auto command_line_args = options.parse(argc, argv);

  if (command_line_args.count("help")) {
    std::cout << options.help({""}) << std::endl;
    exit(0);
  }

  if ( ! command_line_args.count("infile") ) {
    std::cerr << "No input file specified\n";
    exit(1);
  }
  std::string input_file_name = command_line_args["infile"].as<std::string>();

  std::chrono::high_resolution_clock::time_point t1;
  std::chrono::high_resolution_clock::time_point t2;
  std::chrono::duration<double> time_span;

  t1 = std::chrono::high_resolution_clock::now();
  Replay replay(input_file_name);
  replay.build();
  t2 = std::chrono::high_resolution_clock::now();

  if (command_line_args.count("time")) {
    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "Parsing replay log took " << time_span.count() << " seconds." << std::endl;
  }

  t1 = std::chrono::high_resolution_clock::now();
  replay.run();
  t2 = std::chrono::high_resolution_clock::now();

  if (command_line_args.count("time")) {
    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "Running replay took " << time_span.count() << " seconds." << std::endl;
  }

  return 0;
}
