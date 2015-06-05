#include "funs.h"

#include <functional>
#include <exception>
#include <stdexcept>
#include <algorithm> 
#include <iterator>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cctype>
#include <locale>
#include <vector>
#include <bitset>
#include <map>

extern "C" {
#include <fnmatch.h>
#include <unistd.h>
#include <getopt.h>
#include <libgen.h>
#include <ftw.h>
}

#define VERSION "0.1"
#define AUTHOR  "Duane Bekaert"

#ifdef __APPLE__
#define COWPATH "/opt/local/share/cowsay/cows"
//#else defined __SOMETHING__
#else
#define COWPATH "/user/share/cow"
#endif
#define COWFILE "default"

typedef std::map<std::string, std::string> rep_map;
typedef rep_map::iterator it_rep_map;

unsigned int message_word_count = 0;
unsigned int wrap = 40;
std::string eyes = "oo";
std::string tongue = "  ";
std::string cowpath = COWPATH;
std::string cowfile = COWFILE;
std::string message = "";

std::vector<std::string> cowlist = {};

/* functions */
std::string multi_replace(std::string & str, rep_map e_rep_map) {
    for ( it_rep_map it = e_rep_map.begin(); it != e_rep_map.end(); it++) {
        for( size_t pos = 0; ; pos += it->second.length() ) {
            pos = str.find( it->first, pos );
            if( pos == std::string::npos ) 
                break;
            str.erase( pos, it->first.length() );
            str.insert( pos, it->second);
        }
    }
    return str;
}

size_t utf8_size(const std::string & str) {
    size_t counter = 0;
    unsigned int utf_counter = 0;
    for (int i = 0; i < str.size(); i++) {
        auto c = str[i];
        auto mask = 0xC0;
        if ((c & 0xC0) == 0xC0) {
            utf_counter = 1;
        } else if ((c & 0xE0) == 0xE0) {
            utf_counter = 2;
        } else if ((c & 0xF0) == 0xF0) {
            utf_counter = 3;
        } else if ((c & 0xF8) == 0xF8) {
            utf_counter = 4;
        } else if ((c & 0xFC) == 0xFC) {
            utf_counter = 5;
        } else if ((c & 0x80) == 0x80) {
            utf_counter--;
            if (utf_counter == 0)
                counter++;
        } else if (c == 0x1B) {
            while (str[i] != 'm') {
                //if (str[i]+1 > str.size()) break;
                i++;
            }
        } else counter++;
    }
    return counter;
}

std::string latest_escape_sequence(const std::string & str) {
    std::string ret = "";
    for (int i = 0; i < str.size(); i++) {
        auto c = str[i];
        if (c == 0x1B) {
            while (str[i] != 'm') {
                ret += str[i];
                i++;
            }
            ret += 'm';
        }
    }
    return ret;
}

/* simple and efficient trim funcions, from : http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring */
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(),
                s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), 
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

void add_word_to_message(const std::string & word) {
    if (word.empty()) return;
    
    if (message_word_count > 0)
        message += " ";
    message_word_count++;
    message += word;
}

void get_interactive_message() {
    std::cin >> std::noskipws;
    std::istream_iterator<char> it(std::cin);
    std::istream_iterator<char> end;
    std::string results(it, end);

    rep_map replace_list = {
        {"\n", " "},
        {"  ", " "} };

    results = multi_replace(results, replace_list);
    
    message = trim(results);
}

std::vector<std::string> break_down_message() {
    std::string str = message;
    std::vector<std::string> ret;

    unsigned int    counter = 0,
                    prev_index = 0,
                    index = 0; 

    while (index < utf8_size(message)) {
        index += wrap;
        if (index > utf8_size(message)) {
            std::string tmp = str.substr(prev_index);
            tmp = trim(tmp);
            ret.push_back(tmp);
            break;
        }
        while(message[index]!=' ') {
            index--;
        }
        std::string tmp = str.substr(prev_index, index-prev_index);
        tmp = trim(tmp);
        ret.push_back(tmp);
        prev_index = index;
    }
    return ret;
}

void display_usage(const std::string & exname) {
    std::cout << "cxx{say,think} version " << VERSION << 
            ", (c) 2014 Duane Bekaert" << std::endl << "Usage: " << 
            exname << " [-bdgpstwy] [-h] [-e eyes] [-f cowfile] " << 
            std::endl << 
            "          [-l] [-n] [-T tongue] [-W wrapcolumn] [message] " << 
            std::endl;
}

int ftw_callback(const char *fpath, const struct stat *sb, int typeflag) {
    if (FTW_F == typeflag) {
        std::string sfpath = basename((char*)fpath);
        if (sfpath[0] != '.') {
            sfpath = sfpath.replace(sfpath.length() - 4, 4, "");
            cowlist.push_back(sfpath);
        }
    }
    return 0;
}

void display_cow_list() {
    std::cout << "Cow files in " << cowpath << ":" << std::endl;
    ftw(cowpath.c_str(), ftw_callback, 1);
    std::sort(cowlist.begin(), cowlist.end(), std::less<std::string>());
    int ccounter = 0;
    for(auto s : cowlist) {
        if (ccounter + s.length() > 80) {
            std::cout << std::endl;
            ccounter = 0;
        }
        std::cout << s << " ";
        ccounter += s.length()+1;
    }
    std::cout << std::endl;
}

void display_cow(const std::string & exname, RunType type) {
    std::ifstream ifs(cowpath + "/" + cowfile + ".cow");
    if (!ifs.good()) {
        std::cout << exname << ": Could not find " << cowfile << 
                " cowfile!" << std::endl;
        ifs.close();
        return;
    }

    std::string thought, left_bubble, right_bubble;
    if (type == RunType::THINK) {
        thought = "o";
        left_bubble = "( ";
        right_bubble = " )";
    } else {
        thought = "\\";
        left_bubble = "< ";
        right_bubble = " >";
    }

    std::cout << " ";
    if (utf8_size(message) < wrap) {
        for(int i = 0; i < utf8_size(message) + 2; i++)
            std::cout << "_";
        std::cout << std::endl << left_bubble << message << right_bubble << std::endl << " ";
        for(int i = 0; i < utf8_size(message) + 2; i++)
            std::cout << "-";
    } else {
        unsigned int max_len = 0;
        auto v = break_down_message();
        for(auto l : v)
            if (utf8_size(l) > max_len)
                max_len = utf8_size(l);
        
        std::string les = latest_escape_sequence(v[0]); // Last escape sequence
        if (v.size() >= 2) {
            for(int i = 0; i < max_len + 2; i++)
                std::cout << "_";
            int eflag = latest_escape_sequence(v[0]).compare("");
            std::string les;
            std::cout << std::endl;
            for(int index = 0; index < v.size(); index++) {
                if (index == 0) std::cout << "/ ";
                else if (index == v.size()-1) std::cout << "\\ ";
                else std::cout << "| ";

                if (index !=0) {
                    std::string tmples = latest_escape_sequence(v[index-1]);
                    if (tmples.compare("")!=0)
                        les = tmples;

                    if (les.compare("")!=0) {
                        eflag++;
                        std::cout << les;
                    }
                }
                
                std::cout << v[index];
                int r = max_len-utf8_size(v[index]);
                while(r) {
                    std::cout << " ";
                    r--;
                }

                if (eflag) printf("%c[%s", 0x1B, "00m");
                if (index == 0) std::cout << " \\";
                else if (index == v.size()-1) std::cout << " /";
                else std::cout << " |";
                std::cout << std::endl;
            }
            std::cout << " ";
            for(int i = 0; i < max_len + 2; i++)
                std::cout << "-";
        }
    }
    std::cout << std::endl;

    rep_map replace_list = {
            {"\\\\", "\\"},
            {"\\@", "@"},
            {"$thoughts", thought},
            {"$eyes", eyes},
            {"$tongue", tongue} };

    std::string out_str;
    while (!ifs.eof()) {
        std::getline(ifs, out_str);
        if (out_str.substr(0,2).compare("##") == 0) continue;

        if (out_str.find("EOC") == std::string::npos) {
            out_str = multi_replace(out_str, replace_list);
            std::cout << out_str << std::endl;
        }
    }
    std::cout << "\e[A";
}

/* main */
int cowsay(int argc, const char *argv[], RunType type) {
    int opt;
    auto cp = getenv("COWPATH");
    if (cp) cowpath = cp;

    while ((opt = getopt (argc, (char **)argv, "hlne:f:T:W:bdgpstwy")) != -1)
        switch (opt) {
            case 'h':
                display_usage(argv[0]);
                return EXIT_SUCCESS;
            case 'l':
                display_cow_list();
                return EXIT_SUCCESS;
            case 'e':
                eyes.clear();
                eyes += optarg[0];
                eyes += optarg[1];
                break;
            case 'f':
                cowfile = optarg;
                break;
            case 'T':
                tongue.clear();
                tongue += optarg[0];
                tongue += optarg[1];
                break;
            case 'W':
                try {
                    wrap = std::stoi(optarg);
                } catch (std::exception e) {
                    wrap = 1;
                }
                break;
            case 'b':
                eyes = "==";
                break;
            case 'd':
                eyes = "xx";
                tongue = "U ";
                break;
            case 'g':
                eyes = "$$";
                break;
            case 'p':
                eyes = "@@";
                break;
            case 's':
                eyes = "**";
                tongue = "U ";
                break;
            case 't':
                eyes = "--";
                break;
            case 'w':
                eyes = "OO";
                break;
            case 'y':
                eyes = "..";
                break;
            default:
                break;
        }

        for (int index = optind; index < argc; index++)
            add_word_to_message( argv[index]);
        if (message_word_count==0)
            get_interactive_message(); 
        display_cow(argv[0], type);
    return EXIT_SUCCESS;
}

