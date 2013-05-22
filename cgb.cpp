#include <iostream>
#include <iomanip>
#include <string>
// #include <cctype>
#include <locale>
#include <vector>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include "boost/program_options.hpp"
#include <boost/algorithm/string/classification.hpp>
// #include <locale>  //not being used currently - will be later

using boost::lexical_cast;
using namespace std;
using namespace boost;

const unsigned long int GiB = 1073741824;
const unsigned long int MiB = 1048576;
const unsigned long int KiB = 1024;

const string progv = "cgb 0.75";
const string bugaddy = "<git.lamashtu@gmail.com>";


namespace 
{ 
  const size_t ERROR_IN_COMMAND_LINE = 1; 
  const size_t SUCCESS = 0; 
  const size_t ERROR_UNHANDLED_EXCEPTION = 2; 
 
} // namespace 
locale loc;

int main(int ac, char** av) {

	try  { // Define and parse the program options 
		namespace po = boost::program_options;
    		po::options_description desc("Options"); 
    		desc.add_options() 
      		("help", "Print help messages") 
      		("KiB,k", "display result in KiB") 
      		("MiB,m", "display result in MiB")
      		("GiB,g", "display result in GiB"); 

 		po::variables_map vm;
    		try { 
      			po::store(po::parse_command_line(ac, av, desc), vm); // can throw 
 	      		/** --help option */ 
      			if ( vm.count("help") || ac == 1) { 
        			std::cout << std::endl << "cgb" << " [OPTION] NUMBER1 .. [NUMBER N]" << std::endl << desc << std::endl << std::endl << progv << " - Compute GigaBytes:  A kluge that accepts numerical input and spits out the value in Gigabytes, Megabytes, or Kilobytes." << std::endl << std::endl << "Send bug reports to: " << bugaddy; 
        			return SUCCESS; 
      			} //if --help 
 
      		po::notify(vm); // throws on error, so do after help in case 
                      // there are any problems 
    		} //try
    		catch(po::error& e) { 
      			std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
      			std::cerr << desc << std::endl; 
      			return ERROR_IN_COMMAND_LINE; 
    		} //catch errs
 
                if (vm.count("GiB")) {
                        std::vector<std::string> args(av, av+ac);
			int i;
                        for (i = 2; i < ac; i++) {
                                args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
                                double r = lexical_cast<double>(args[i]) / (double)GiB;
                                std::cout << i << ": " << setiosflags(ios::fixed) << setprecision(2) << r << "GiB" << std::endl;
                        } //for
                } //if vm.count
                else if (vm.count("MiB")) {
			std::vector<std::string> args(av, av+ac);
                        int i;
                        for (i = 2; i < ac; i++) {
                                args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
                                double r = lexical_cast<double>(args[i]) / (double)MiB;
                                std::cout << i << ": " << setiosflags(ios::fixed) << setprecision(2) << r << "MiB" << std::endl;
                        } //for
                } //else if m
                else if (vm.count("KiB")) {
                        std::vector<std::string> args(av, av+ac);
			int i;
                        for (i = 2; i < ac; i++) {
                                args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
                                double r = lexical_cast<double>(args[i]) / (double)KiB;
                                std::cout << i << ": " << setiosflags(ios::fixed) << setprecision(2) << r << "KiB" << std::endl;
                        } //for
                } //else if k
                else {
                        std::vector<std::string> args(av, av+ac);
			int i;
                        for (i = 1; i < ac; i++) {
                                args[i].erase(std::remove_if(args[i].begin(), args[i].end(), !is_digit()), args[i].end());
                                double r = lexical_cast<double>(args[i]) / (double)GiB;
                                std::cout << i << ": " << setiosflags(ios::fixed) << setprecision(2) << r << "GiB" << std::endl;
                        } //for
                } //else - defaulting to GiB
  	} //try 
	
	catch(std::exception& e) { 
    		std::cerr << "Unhandled Exception reached the top of main: " << e.what() << ", application will now exit" << std::endl; 
		return ERROR_UNHANDLED_EXCEPTION; 
 	} //catch
 
	return SUCCESS; 


} //main
