/*The MIT License (MIT)

Copyright (c) 2013 David Ryack

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "cgb.h"

int main(int ac, char** av) {
	int i = 1; // outside the for() loops so it can be modified when -p and/or -e are in use
	int n = 1; // for output formatting independent of i in for() loops

	try  { // Define and parse the program options 
		namespace po = boost::program_options;
    		po::options_description desc("Options"); 
    		desc.add_options() 
      		("help", "Print help message") 
      		("KiB,k", "display result in KiB") 
      		("MiB,m", "display result in MiB")
      		("GiB,g", "display result in GiB")
		("enum,e", "enumerate results")
		("precision,p",po::value<unsigned int>(&prec), "output results with precision of n decimal places");

 		po::variables_map vm;
    		try { 
      			po::store(po::parse_command_line(ac, av, desc), vm); // can throw 
 	      		/** --help option */ 
      			if ( vm.count("help") || ac == 1) { 
        			std::cout << std::endl << "cgb" << " [-kmg] [--precision|-p]<arg> NUMBER1 .. [NUMBER N]" << std::endl << desc << std::endl << std::endl \
				 << progv << " - Compute GigaBytes:  A kluge that accepts numerical input and spits out the value in Gigabytes, Megabytes, or Kilobytes." \
				 << std::endl << std::endl << "Send bug reports to: " << bugaddy << std::endl;
			return SUCCESS; 
      			} //if --help or no values given for computation
 
      		po::notify(vm); // throws on error, so do after help in case 
                      // there are any problems 
    		} //try
    		
    		catch(po::error& e) { 
      			std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
      			std::cerr << desc << std::endl; 
      			return ERROR_IN_COMMAND_LINE; 
    		} //catch errs
    		
		std::vector<std::string> args(av, av+ac); //vectorize command line for easy processing
		if (vm.count("GiB") && vm.count("MiB") && vm.count("KiB")) {
			if (args[1] == ("-gmk")) {i=2;}  //testing for sticky options
			else if (args[1] == "-kmg") {i=2;}
			else if (args[1] == "-kgm") {i=2;}
			else if (args[1] == "-gkm") {i=2;}
			else if (args[1] == "-mgk") {i=2;}
			else if (args[1] == "-mkg") {i=2;}
			else {i=4;} //indicates non-sticky options, and verified due to entry else if MiB && KiB
			if (vm.count("precision")) { i++;} //account for precision being on the command line
			if (vm.count("enum")) {i++;} //account for enumumerate being on the command line
			//  Currently the following forms all work:
			//	cgb -gmk, cgb -gmkp#, cgb -g -m -k -p#
			//
			//  The following currently fails:
			//	cgb -g -m -kp#
			unsigned int maxlen = 0;
			unsigned int j = i; //holds i prior to 1st for loop so it can be re-used during 2nd for loop
			for (i; i < ac; i++) {
				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
				if (maxlen < args[i].length()) {
				    maxlen = args[i].length();
				}
			}  //clean up args and find the longest 
			i = j; //reset i so 2nd loop will work	
			for (i; i < ac; i++) {
				
// 				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
				double r = lexical_cast<double>(args[i]);
			
				if (vm.count("enum")) {
					resultOut(r, n, GMK, 1, prec, maxlen);
				} //if "enum" on cmd line
				
				else {
					resultOut(r, n, GMK, 0, prec, maxlen);
				} //else enumeration wasn't requested
				n++;
			} //for
		} // if -gmk
		else if (vm.count("GiB") && vm.count("MiB")) {
                        if (args[1] == ("-gm")) {i=2;}  //testing for sticky options
                        else if (args[1] == "-mg") {i=2;}
                        else {i=3;} //indicates non-sticky options, and verified due to entry else if MiB && KiB
                        if (vm.count("precision")) { i++;std::vector<std::string> args(av, av+ac);} //account for precision being on the command line
			if (vm.count("enum")) {i++;}  //acount for enum on cmd line
			
			unsigned int maxlen = 0;
			unsigned int j = i; //holds i prior to 1st for loop so it can be re-used during 2nd for loop
			for (i; i < ac; i++) {
				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
				if (maxlen < args[i].length()) {
				    maxlen = args[i].length();
				}
			}  //clean up args and find the longest 
                        i = j; //reset i so 2nd loop will work
                        for (i; i < ac; i++) {
                        	
// 				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
                                double r = lexical_cast<double>(args[i]);
				
				if (vm.count("enum")) {
				      resultOut(r, n, GM, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                
                                else {  
				      resultOut(r, n, GM, 0, prec, maxlen);
                                } //else enumeration wasn't requested
				n++;
			} //for
		} // if -gm
		else if (vm.count("GiB") && vm.count("KiB")) {
                        if (args[1] == ("-gk")) {i=2;}  //testing for sticky options
                        else if (args[1] == "-kg") {i=2;}
                        else {i=3;} //indicates non-sticky options, and verified due to entry else if MiB && KiB
                        if (vm.count("precision")) { i++;} //account for precision being on the command line
			if (vm.count("enum")) {i++;} //account for enumumerate being on the command line
                        
                        unsigned int maxlen = 0;
			unsigned int j = i; //holds i prior to 1st for loop so it can be re-used during 2nd for loop
			for (i; i < ac; i++) {
				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
				if (maxlen < args[i].length()) {
				    maxlen = args[i].length();
				}
			}  //clean up args and find the longest
			i = j; //reset i so 2nd loop will work
                        for (i; i < ac; i++) {
                                
// 				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
                                double r = lexical_cast<double>(args[i]);
                                
				if (vm.count("enum")) {
                                        resultOut(r, n, GK, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                else {
                                        resultOut(r, n, GK, 0, prec, maxlen);
                                } //else enumeration wasn't requested
				n++;
                        } //for
                } // if -gk
		else if (vm.count("MiB") && vm.count("KiB")) {
                        if (args[1] == ("-mk")) {i=2;}  //testing for sticky options
                        else if (args[1] == "-km") {i=2;}
                        else {i=3;} //indicates non-sticky options, and verified due to entry else if MiB && KiB
                        if (vm.count("precision")) { i++;} //account for precision being on the command line
			if (vm.count("enum")) {i++;} //account for enumumerate being on the command line
			unsigned int maxlen = 0;
			unsigned int j = i; //holds i prior to 1st for loop so it can be re-used during 2nd for loop
			for (i; i < ac; i++) {
				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
				if (maxlen < args[i].length()) {
				    maxlen = args[i].length();
				}
			}  //clean up args and find the longest 
 			i = j; //reset i so 2nd loop will work
                        for (i; i < ac; i++) {
                                
// 				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
                                double r = lexical_cast<double>(args[i]);
				
				if (vm.count("enum")) {
                                        resultOut(r, n, MK, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                else {	
					resultOut(r, n, MK, 0, prec, maxlen);
                                } //else enumeration wasn't requested

                                n++;
                        } //for
                } // if -km
                else if (vm.count("GiB")) {
			if (vm.count("precision")) { i++;} //account for precision being on the command line
			if (vm.count("enum")) {i++;} //account for enumumerate being on the command line
			
			unsigned int maxlen = 0;
			unsigned int j = i; //holds i prior to 1st for loop so it can be re-used during 2nd for loop
			for (i; i < ac; i++) {
				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
				if (maxlen < args[i].length()) {
				    maxlen = args[i].length();
				}
			}  //clean up args and find the longest 
                        i = j; //reset i so 2nd loop will work
                        for (i++; i < ac; i++) {
                                
// 				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
                                double r = lexical_cast<double>(args[i]);
				
				if (vm.count("enum")) {
                                        resultOut(r, n, G, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                else {
                                        resultOut(r, n, G, 0, prec, maxlen);
                                } //else enumeration wasn't requested
				n++;
                        } //for
                } //else if -g
                else if (vm.count("MiB")) {
                        if (vm.count("precision")) { i++;} //account for precision being on the command line
			if (vm.count("enum")) {i++;} //account for enumumerate being on the command line
			
			unsigned int maxlen = 0;
			unsigned int j = i; //holds i prior to 1st for loop so it can be re-used during 2nd for loop
			for (i; i < ac; i++) {
				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
				if (maxlen < args[i].length()) {
				    maxlen = args[i].length();
				}
			}  //clean up args and find the longest 
                        i = j; //reset i so 2nd loop will work
                        for (i++; i < ac; i++) { 
				
// 				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
                                double r = lexical_cast<double>(args[i]);
				
				if (vm.count("enum")) {
                                        resultOut(r, n, M, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                else {  
                                        resultOut(r, n, M, 0, prec, maxlen);
                                } //else enumeration wasn't requested
				n++;
                        } //for
                } //else if m
                else if (vm.count("KiB")) {
			if (vm.count("precision")) { i++;} //account for precision being on the command line
			if (vm.count("enum")) {i++;} //account for enumumerate being on the command line
			
			unsigned int maxlen = 0;
			unsigned int j = i; //holds i prior to 1st for loop so it can be re-used during 2nd for loop
			for (i; i < ac; i++) {
				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
				if (maxlen < args[i].length()) {
				    maxlen = args[i].length();
				}
			}  //clean up args and find the longest 
                        i = j; //reset i so 2nd loop will work
                        for (i++; i < ac; i++) {
                                
// 				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
                                double r = lexical_cast<double>(args[i]);
				
				if (vm.count("enum")) {
                                        resultOut(r, n, K, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                else {  
                                        resultOut(r, n, K, 0, prec, maxlen);
                                } //else enumeration wasn't requested
				n++;
                        } //for
                } //else if k
                else {
			if (vm.count("precision")) { i++;} //account for precision being on the command line
			if (vm.count("enum")) {i++;} //account for enumumerate being on the command line
			
			unsigned int maxlen = 0;
			unsigned int j = i; //holds i prior to 1st for loop so it can be re-used during 2nd for loop
			for (i; i < ac; i++) {
				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
				if (maxlen < args[i].length()) {
				    maxlen = args[i].length();
				}
			}  //clean up args and find the longest 
                        i = j; //reset i so 2nd loop will work
                        for (i; i < ac; i++) {
                        
// 				args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
                                double r = lexical_cast<double>(args[i]);
				
				if (vm.count("enum")) {
                                        resultOut(r, n, G, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                else {  
                                        resultOut(r, n, G, 0, prec, maxlen);
                                } //else enumeration wasn't requested

				n++;
                        } //for
                } //else - defaulting to GiB
  	} //try 
	
	catch(std::exception& e) { 
    		std::cerr << "Unhandled Exception reached the top of main: " << e.what() << ", application will now exit" << std::endl; 
		return ERROR_UNHANDLED_EXCEPTION; 
 	} //catch
 
	return SUCCESS; 


} //main
