#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <string>
#include <iostream>

namespace cxxsay {
    
    void display_usage(std::string exname, std::string e_version);

    void display_cow_list(std::string e_cowpath);

    void display_cow(   std::string e_eyes,
                        std::string  e_tongue,
                        std::string  e_cowpath,
                        std::string  e_cowfile,
                        std::string  e_message);

} /* cxxsay */

#endif /* __DISPLAY_H__ */
