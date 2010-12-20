//----------------------------  parameter_handler_read_xml.cc  ---------------------------
//    $Id$
//    Version: $Name$
//
//    Copyright (C) 2002, 2003, 2004, 2005, 2010 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  parameter_handler_read_xml.cc  ---------------------------


// check ParameterHandler::print_parameters (..., XML). have a few
// names that contain all sorts of weird (for XML) characters

#include "../tests.h"
#include <base/logstream.h>
#include <base/parameter_handler.h>
#include <fstream>


int main ()
{
  std::ofstream logfile("parameter_handler_read_xml/output");
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  ParameterHandler prm;
  prm.declare_entry ("int1",
		     "1",
		     Patterns::Integer(),
		     "doc 1");
  prm.declare_entry ("int2",
		     "2",
		     Patterns::Integer(),
		     "doc 2");
  prm.enter_subsection ("ss1");
  {
    prm.declare_entry ("double 1",
		       "1.234",
		       Patterns::Double(),
		       "doc 3");

    prm.enter_subsection ("ss2");
    {
      prm.declare_entry ("double 2",
			 "4.321",
			 Patterns::Double(),
			 "doc 4");
    }
    prm.leave_subsection ();
  }
  prm.leave_subsection ();

				   // things with strange characters
  prm.enter_subsection ("Testing%testing");
  {
    prm.declare_entry ("string&list",
		       "< & > ; /",
		       Patterns::Anything(),
		       "docs 1");
    prm.declare_entry ("int*int",
		       "2",
		       Patterns::Integer());
    prm.declare_entry ("double+double",
		       "6.1415926",
		       Patterns::Double(),
		       "docs 3");
  }
  prm.leave_subsection ();

				   // read from XML
  std::ifstream in ("parameter_handler_read_xml/prm");
  bool result = prm.read_input_from_xml (in);
  Assert (result == true, ExcInternalError());

				   // write it out again
  prm.print_parameters (deallog.get_file_stream(), ParameterHandler::XML);
  logfile << std::endl;

  return 0;
}
