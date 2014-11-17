#include "display.h"

namespace cxxsay {
    
    void display_usage(std::string exname, std::string e_version) {
        std::cout << "cxx{say,think} version " << e_version << 
                ", (c) 2014 Duane Bekaert" << std::endl << "Usage: " << 
                exname << " [-bdgpstwy] [-h] [-e eyes] [-f cowfile] " << 
                std::endl << 
                "          [-l] [-n] [-T tongue] [-W wrapcolumn] [message] " << 
                std::endl;
    }

    void display_cow_list(std::string e_cowpath) {
        std::cout << "lel" << std::endl;
    }

    void display_cow(   std::string e_eyes,
                        std::string  e_tongue,
                        std::string  e_cowpath,
                        std::string  e_cowfile,
                        std::string  e_message) {
        std::cout << "lel" << std::endl;
    }
} /* display */
