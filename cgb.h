/*The MIT License (MIT)

Copyright (c) 2013-2018 David Ryack

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

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include "boost/program_options.hpp"
#include <boost/algorithm/string/classification.hpp>

using namespace std;
using namespace boost;

namespace
{
  const size_t ERROR_IN_COMMAND_LINE = 1;
  const size_t SUCCESS = 0;
  const size_t ERROR_UNHANDLED_EXCEPTION = 2;

} // namespace 

locale loc;


const double GiB = 1073741824.0;
const double MiB = 1048576.0;
const double KiB = 1024.0;
unsigned int prec = 2; //default precision
const string progv = "cgb 0.99f";
const string bugaddy = "<git.lamashtu@gmail.com>";
const string licenseS = " The MIT License (MIT)\n Copyright (c) 2013-2018 David Ryack\n \
Permission is hereby granted, free of charge, to any person obtaining a copy of\n \
this software and associated documentation files (the \"Software\"), to deal in\n \
the Software without restriction, including without limitation the rights to\n \
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of\n \
the Software, and to permit persons to whom the Software is furnished to do so,\n \
subject to the following conditions:\n\n \
The above copyright notice and this permission notice shall be included in all\n \
copies or substantial portions of the Software.\n\n \
THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n \
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS\n \
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR\n \
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER\n \
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN\n \
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n";

enum simode_t {GMK = 0, GM = 1, GK = 2, MK = 3, M = 4, K = 5, G = 6};

//////////////////////////////////////////////////////////////////////////////
//  This function is accepting too many params; need to break it up at the
//  very least.  Even better would be if it wasn't so long and hideous
//////////////////////////////////////////////////////////////////////////////

void resultOut(double x, int y, int z, int a, unsigned int b, unsigned int c) {
    //x = argss[i] passed via r
    //y = i
    //z == simode_t above
    //a == 0 standard output, 1 enumerated output
    //b == prec(ision)
    //c == maxlen
    
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
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << r;
	    std::cout << setw(c+4+b) << setiosflags(ios::fixed) << setprecision(b) << s << std::endl;
	}
	else if (z == 2) { //--GiB --KiB
	    double r = x / GiB;
	    double s = x / KiB;
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << r;
	    std::cout << setw(c+4+b) << setiosflags(ios::fixed) << setprecision(b) << s << std::endl;
	}
	else if (z == 1) { //--GiB --MiB
	    double r = x / GiB;
	    double s = x / MiB;
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << r;
	    std::cout << setw(c+4+b) << setiosflags(ios::fixed) << setprecision(b) << s << std::endl;
	}
	else if (z == 0) { //--GiB --MiB --KiB
	    double r = x / GiB;
	    double s = x / MiB;
	    double t = x / KiB;
	    std::cout << setiosflags(ios::fixed) << setprecision(b) << r;
	    std::cout << setw(c+4+b) << setiosflags(ios::fixed) << setprecision(b) << s;
	    std::cout << setw(c+4+b) << setiosflags(ios::fixed) << setprecision(b) << t << std::endl;
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
	    std::cout << y << ": " << setiosflags(ios::fixed) << setprecision(b) << r << "MiB";
	    std::cout << setw(c+6+b) << setiosflags(ios::fixed) << setprecision(b) << s << "KiB" << std::endl;
	}
	else if (z == 2) { //--GiB --KiB
	    double r = x / GiB;
	    double s = x / KiB;
	    std::cout << y << ": " << setiosflags(ios::fixed) << setprecision(b) << r << "GiB";
	    std::cout << setw(c+6+b) << setiosflags(ios::fixed) << setprecision(b) << s << "KiB" << std::endl;
	}
	else if (z == 1) { //--GiB --MiB
	    double r = x / GiB;
	    double s = x / MiB;
	    std::cout << y << ": " << setiosflags(ios::fixed) << setprecision(b) << r << "GiB";
	    std::cout << setw(c+6+b) << setiosflags(ios::fixed) << setprecision(b) << s << "MiB" << std::endl;
	}
	else if (z == 0) { //--GiB --MiB --KiB
	    double r = x / GiB;
	    double s = x / MiB;
	    double t = x / KiB;
	    std::cout << y << ": " << setiosflags(ios::fixed) << setprecision(b) << r << "GiB";
	    std::cout << setw(c+6+b) << setiosflags(ios::fixed) << setprecision(b) << s << "MiB";
	    std::cout << setw(c+6+b) << setiosflags(ios::fixed) << setprecision(b) << t << "KiB" << std::endl;
	}
	else {
	  std::cerr << "Unsupported arithmetic request." << std::endl;
	}
    }
    else {
	std::cerr  << "Unsupported formatting request." << std::endl;
    }
}

// A helper function to simplify the main part.
// for output of positional arguments -- code provided by boost program options examples
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(os, " ")); 
    return os;
}

void removeEmptyStrings(vector<string>& strings)
{
  vector<string>::iterator it = remove_if(strings.begin(), strings.end(),mem_fun_ref(&string::empty));
  // erase the removed elements
  strings.erase(it, strings.end());
}  //removeEmptyStrings()
	
double castDouble(std::string i) {
	double r = lexical_cast<double>(i);
	return r;
}  //castDouble()


////////////////////////////////////////
//  Pretty ugly... can we improve it?
//
//  Since we're successfully passing
//  the command line args, this is 
//  also a reminder that almost all
//  the code in cgp.cpp can be nicely
//  broken up.
////////////////////////////////////////
simode_t getSIOption(boost::program_options::variables_map Map) {
	if (Map.count("GiB") && Map.count("MiB") && Map.count("KiB")) {
		return GMK;
	}
	else if (Map.count("GiB") && Map.count("MiB")) {
		return GM;
	}
	else if (Map.count("GiB") && Map.count("KiB")) {
		return GK;
	}
	else if (Map.count("MiB") && Map.count("KiB")) {
		return MK;
	}
	else if (Map.count("GiB")) {
		return G;
	}
	else if (Map.count("MiB")) {
		return M;
	}
	else if (Map.count("KiB")) {
		return K;
	}
	else {
		return G;
	}
} // getSIOption()
