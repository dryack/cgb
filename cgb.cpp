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
	unsigned int i = 0; // outside the for() loops so it can be modified when -p and/or -e are in use
	unsigned int maxlen = 0; //max length of positional arguments; needed for formatting
	std::vector<std::string> argss; //vector that will be holding arguments (values to be computed)
	
	try  { // Define and parse the program options 
		namespace po = boost::program_options;
    		po::options_description desc("Options"); 
    		desc.add_options() 
      		("help", "print help message") 
		("license", "display license")
      		("KiB,k", "display result in KiB") 
      		("MiB,m", "display result in MiB")
      		("GiB,g", "display result in GiB")
		("enum,e", "enumerate results")
		("precision,p",po::value<unsigned int>(&prec), "output results with precision of n decimal places");
		
		po::options_description hidden("Hidden options");
		hidden.add_options()
		("compute-value", po::value< vector<string> >(), "values to be computed");        
		
		po::positional_options_description p;
		p.add("compute-value", -1);
		
		po::options_description cmdline_options;
		cmdline_options.add(desc).add(hidden);
		
		po::variables_map vm;
    		try { 
      			po::store(po::command_line_parser(ac, av).options(desc).options(cmdline_options).positional(p).run(), vm);
			// can throw 
 	      		//if --help is requested OR the commandline is empty AND no data is being piped in (isatty() )
      			if ( vm.count("help") || (ac == 1 && isatty(fileno(stdin)))) {  
        			std::cout << std::endl << "cgb" << " [-kmg] [--precision|-p]<int> NUMBER1 .. [NUMBER N]" << std::endl << desc << std::endl << std::endl \
				 << progv << " - Compute GigaBytes:  A kluge that accepts numerical input and spits out the value in Gigabytes, Megabytes, or Kilobytes." \
				 << std::endl << std::endl << "Send bug reports to: " << bugaddy << std::endl;
				 return SUCCESS;
			} //if --help or no values given for computation
			else if (vm.count("license")){
				std::cout << std::endl << " " << progv << std::endl << std::endl << licenseS << std::endl;
				return SUCCESS;
			}
			       			 
			po::notify(vm); // throws on error, so do after help in case 
				      // there are any problems 
    		} //try
    		
    		catch(po::error& e) { 
      			std::cerr << "\nERROR : " << e.what() << std::endl << std::endl; 
      			std::cerr << desc << std::endl; 
      			return ERROR_IN_COMMAND_LINE; 
    		} //catch errs
    		
    		if (!isatty(fileno(stdin))) { //checking for piped
			std::string blah;
 			while (getline(cin, blah, '\n')) { //possible candidate for future options, changing getline behavior
				argss.push_back(blah);
			}
			for (i; i < argss.size(); i++) {
			//strip each positional argument of all non digits
			//stripping spaces, probably not needed under 99% of cases; was required during more extreme tests
			argss[i].erase(std::remove_if(argss[i].begin(), argss[i].end(), is_space()), argss[i].end());
			argss[i].erase(std::remove_if(argss[i].begin(), argss[i].end(), !is_digit()), argss[i].end());
				if (maxlen < argss[i].length()) {
					maxlen = argss[i].length();
				}
			}
			removeEmptyStrings(argss); //another function mostly needed due to extreme cases
 		}
		else {
     		//vectorize positional arguments
			std::vector<std::string> argsst(vm["compute-value"].as< vector<string> >());
			
			for (i; i < argsst.size(); i++) {
				//strip each positional argument of all non digits
				argsst[i].erase(std::remove_if(argsst[i].begin(), argsst[i].end(), !is_digit()), argsst[i].end());
				if (maxlen < argsst[i].length()) {
					maxlen = argsst[i].length();
				}
			}
			argss.assign(argsst.begin(),argsst.end()); //due to scoping, this version of the vector
								   //needs to be copied to argss
			removeEmptyStrings(argss);
		}
		
		
		i = 0;  //reset i for future iterations
		
		if (vm.count("GiB") && vm.count("MiB") && vm.count("KiB")) {
			for (i; i < argss.size(); i++) {
				double r = lexical_cast<double>(argss[i]);
				if (vm.count("enum")) {
					resultOut(r, i, GMK, 1, prec, maxlen);
				} //if "enum" on cmd line
				
				else {
					resultOut(r, i, GMK, 0, prec, maxlen);
				} //else enumeration wasn't requested
			} //for
		} // if -gmk
		else if (vm.count("GiB") && vm.count("MiB")) {
                        for (i; i < argss.size(); i++) {
                                double r = lexical_cast<double>(argss[i]);
				if (vm.count("enum")) {
				      resultOut(r, i, GM, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                
                                else {  
				      resultOut(r, i, GM, 0, prec, maxlen);
                                } //else enumeration wasn't requested
			} //for
		} // if -gm
		else if (vm.count("GiB") && vm.count("KiB")) {
                        for (i; i < argss.size(); i++) {
                                double r = lexical_cast<double>(argss[i]);
				if (vm.count("enum")) {
                                        resultOut(r, i, GK, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                else {
                                        resultOut(r, i, GK, 0, prec, maxlen);
                                } //else enumeration wasn't requested
	                } //for
                } // if -gk
		else if (vm.count("MiB") && vm.count("KiB")) {
                        for (i; i < argss.size(); i++) {
                                double r = lexical_cast<double>(argss[i]);
				if (vm.count("enum")) {
                                        resultOut(r, i, MK, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                else {	
					resultOut(r, i, MK, 0, prec, maxlen);
                                } //else enumeration wasn't requested
                        } //for
                } // if -km
                else if (vm.count("GiB")) {
                        for (i; i < argss.size(); i++) {
                                double r = lexical_cast<double>(argss[i]);
				
				if (vm.count("enum")) {
                                        resultOut(r, i, G, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                else {
                                        resultOut(r, i, G, 0, prec, maxlen);
                                } //else enumeration wasn't requested
                        } //for
                } //else if -g
                else if (vm.count("MiB")) {
                       for (i; i < argss.size(); i++) {
				double r = lexical_cast<double>(argss[i]);
				
				if (vm.count("enum")) {
                                        resultOut(r, i, M, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                else {  
                                        resultOut(r, i, M, 0, prec, maxlen);
                                } //else enumeration wasn't requested
			} //for
                } //else if m
                else if (vm.count("KiB")) {
			for (i; i < argss.size(); i++) {
				double r = lexical_cast<double>(argss[i]);
				
				if (vm.count("enum")) {
                                        resultOut(r, i, K, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                else {  
                                        resultOut(r, i, K, 0, prec, maxlen);
                                } //else enumeration wasn't requested
				
                        } //for
                } //else if k
                else {
			for (i; i < argss.size(); i++) {
                        
                                double r = lexical_cast<double>(argss[i]);
						
				if (vm.count("enum")) {
                                        resultOut(r, i, G, 1, prec, maxlen);
                                } //if "enum" on cmd line
                                else {  
                                        resultOut(r, i, G, 0, prec, maxlen);
                                } //else enumeration wasn't requested
                        } //for
                } //else - defaulting to GiB
  	} //try 
	
	catch(std::exception& e) { 
    		std::cerr << "\nUnhandled Exception reached the top of main: " << e.what() << ", application will now exit" << std::endl; 
		return ERROR_UNHANDLED_EXCEPTION; 
 	} //catch
 
	return SUCCESS; 


} //main
