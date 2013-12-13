//cgb.h


//resultOut()
//needs to know r (from the for loop, the value to be displayed)
//needs to know n (from the for loop, for enumeration)
//needs to know output style

//double r = lexical_cast<double>(args[i])
#include <iostream>
#include <iomanip>
#include <string>
#include <locale>
#include <vector>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include "boost/program_options.hpp"
#include <boost/algorithm/string/classification.hpp>
#include <iterator>
#include <boost/concept_check.hpp>

using boost::lexical_cast;
using namespace std;
using namespace boost;

const double GiB = 1073741824.0;
const double MiB = 1048576.0;
const double KiB = 1024.0;
unsigned int prec = 2; //default precision
const string progv = "cgb 0.98";
const string bugaddy = "<git.lamashtu@gmail.com>";

enum simode_t {GMK = 0, GM = 1, GK = 2, MK = 3, M = 4, K = 5, G = 6};

void resultOut(double x, int y, int z, int a, unsigned int b) {
    //x = args[i] passed via r
    //y = n
  
    //z == 0 GMK
    //z == 1 GM
    //z == 2 GK
    //z == 3 MK
    //z == 4 M
    //z == 5 K
    //z == 6 G <--- default case
  
    //a == 0 standard output
    //a == 1 emumerated output
  
    //b == precision
    
    if (a == 0) {  //default output
        if (z == 6) {  //default request --GiB
	    double r = x / GiB;
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << r << std::endl;
	} // --GiB
	else if (z == 5) { //--KiB
	    double r = x / KiB;
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << r << std::endl;
	}
	else if (z == 4) { // --MiB
	    double r = x / MiB;
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << r << std::endl;
	}
	else if (z == 3) { //--MiB --KiB
	    double r = x / MiB;
	    double s = x / KiB;
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << r << "\t\t\t";
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << s << std::endl;
	}
	else if (z == 2) { //--GiB --KiB
	    double r = x / GiB;
	    double s = x / KiB;
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << r << "\t\t\t";
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << s << std::endl;
	}
	else if (z == 1) { //--GiB --MiB
	    double r = x / GiB;
	    double s = x / MiB;
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << r << "\t\t\t";
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << s << std::endl;
	}
	else if (z == 0) { //--GiB --MiB --KiB
	    double r = x / GiB;
	    double s = x / MiB;
	    double t = x / KiB;
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << r << "\t\t\t";
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << s << "\t\t\t";
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << t << std::endl;
	}
	else {
	  
	    std::cerr << "Unsupported arithmetic request." << std::endl;
	}
    }
    else if (a == 1) { //enumerated output requested
	if (z == 6) {  //default request --GiB
	    double r = x / GiB;
	    std::cout << y << ": " << setiosflags(ios::fixed) << setprecision(b) << r << "GiB" << std::endl;
	} // --GiB
	else if (z == 5) { //--KiB
	    double r = x / KiB;
	    std::cout << y << ": " << setiosflags(ios::fixed) << setprecision(b) << r << "KiB" << std::endl;
	}
	else if (z == 4) { // --MiB
	    double r = x / MiB;
	    std::cout << y << ": " << setiosflags(ios::fixed) << setprecision(b) << r << "MiB" << std::endl;
	}
	else if (z == 3) { //--MiB --KiB
	    double r = x / MiB;
	    double s = x / KiB;
	    std::cout << y << ": " << setiosflags(ios::fixed) << setprecision(b) << r << "MiB" << "\t\t\t";
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << s << "KiB" << std::endl;
	}
	else if (z == 2) { //--GiB --KiB
	    double r = x / GiB;
	    double s = x / KiB;
	    std::cout << y << ": " << setiosflags(ios::fixed) << setprecision(b) << r << "GiB" << "\t\t\t";
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << r << "KiB" << std::endl;
	}
	else if (z == 1) { //--GiB --MiB
	    double r = x / GiB;
	    double s = x / MiB;
	    std::cout << y << ": " << setiosflags(ios::fixed) << setprecision(b) << r << "GiB" << "\t\t\t";
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << s << "MiB" << std::endl;
	}
	else if (z == 0) { //--GiB --MiB --KiB
	    double r = x / GiB;
	    double s = x / MiB;
	    double t = x / KiB;
	    std::cout << y << ": " << setiosflags(ios::fixed) << setprecision(b) << r << "GiB" << "\t\t\t";
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << s << "MiB" << "\t\t\t";
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << t << "KiB" << std::endl;
	}
	else {
	  std::cerr << "Unsupported arithmetic request." << std::endl;
	}
    }
    else {
	std::cerr  << "Unsupported formatting request." << std::endl;
    }
}
    
	
    
