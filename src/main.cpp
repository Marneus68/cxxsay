#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

extern "C" {
    #include <unistd.h>
    #include <getopt.h>
}

#include "display.h"

const std::string version = "0.1";

unsigned int wrap = 40;
std::string eyes = "oo";
std::string tongue = "  ";
std::string mouth = "  ";
std::string cowpath = "/usr/share/cow";
std::string cowfile = "default.cow";
std::string message = "";

int
main(int argc, const char *argv[])
{
    int opt;

    while ((opt = getopt (argc, (char **)argv, "hlne:f:T:W:")) != -1)
        switch (opt) {
            case 'h':
                cxxsay::display_usage(argv[0], version);
                return EXIT_SUCCESS;
            case 'l':
                std::cout << "Cow files in " << 
                        cowpath << ":" << std::endl;
                return EXIT_SUCCESS;
            case 'e':
                //if (eyes.size() == 1) {
                eyes.clear();
                eyes += optarg[0];
                eyes += optarg[1];
                break;
            case 'f':
                cowfile = optarg;
                break;
            case 'T':
                mouth.clear();
                mouth += optarg[0];
                mouth += optarg[1];
                break;
            case 'W':
                try {
                    wrap = std::stoi(optarg);
                } catch (std::exception e) {
                    wrap = 1;
                }
                break;
            default:
                break;
        }
    return EXIT_SUCCESS;
}

