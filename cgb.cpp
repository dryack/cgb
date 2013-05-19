#include <iostream>
#include <iomanip>
#include <string>
// #include <cctype>
#include <locale>
#include <vector>
#include <algorithm>
#include <boost/lexical_cast.hpp>
// #include <boost/algorithm/string/classification.hpp>
// #include <locale>  //not being used currently - will be later

using boost::lexical_cast;
using namespace std;
using namespace boost;

const unsigned long int GiB = 1073741824;
const unsigned long int MiB = 1048576;
const unsigned long int KiB = 1024;

int main(int ac, char* av[]) {
		locale loc;

        if (ac > 1) {
                std::vector<std::string> args(av, av+ac);
                int i;
                for (i = 1; i < ac; i++) {
                        //std::cout << "av[" << i << "] = " << args[i] << '\n';
                        if (std::count(args[i].begin(), args[i].end(), ',') > 0) {
                                args[i].erase(std::remove(args[i].begin(), args[i].end(), ','), args[i].end());
                                double r = lexical_cast<double>(args[i]) / (double)GiB;
                                //std::cout << "after trimming, av[" << i << "] = " << args[i] << std::endl;
                                std::cout << i << ": " << setiosflags(ios::fixed) << setprecision(2) << r << "GiB" << std::endl;
                        }
                        else {
                                double r = lexical_cast<double>(args[i]) / (double)GiB;
                                std::cout << i << ": " << setiosflags(ios::fixed) << setprecision(2) << r << "GiB" << std::endl;
                                //std::cout << "No commas found in av[" << i << "]" << std::endl;
                        }
                }
        }
        else
                std::cout << "Usage: " << av[0] << " <NUMBER..>" << std::endl;

}
