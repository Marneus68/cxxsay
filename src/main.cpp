#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <exception>
#include <stdexcept>
#include <functional>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <vector>
#include <map>

extern "C" {
    #include <unistd.h>
    #include <getopt.h>
}

typedef std::map<std::string, std::string> rep_map;
typedef rep_map::iterator it_rep_map;

/* constants */
const std::string version = "0.1";

/* variables */
unsigned int message_word_count = 0;

unsigned int wrap = 40;
std::string eyes = "oo";
std::string tongue = "  ";
//std::string cowpath = "/usr/share/cow";
std::string cowpath = "/opt/local/share/cowsay/cows";
std::string cowfile = "default";
std::string message = "";

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

void print_padding(const std::string & line) {
    for(int i = line.size(); i < wrap-1; i++) {
        std::cout << " ";
    }
}

std::vector<std::string> break_down_message() {
    std::string str = message;
    std::vector<std::string> ret;

    unsigned int    counter = 0,
                    prev_index = 0,
                    index = 0; 

    while (index < message.size()) {
        index += wrap;
        if (index > message.size()) {
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
    std::cout << "cxx{say,think} version " << version << 
            ", (c) 2014 Duane Bekaert" << std::endl << "Usage: " << 
            exname << " [-bdgpstwy] [-h] [-e eyes] [-f cowfile] " << 
            std::endl << 
            "          [-l] [-n] [-T tongue] [-W wrapcolumn] [message] " << 
            std::endl;
}

void display_cow_list(std::string e_cowpath) {
}

void display_cow(const std::string & exname) {
    std::ifstream ifs(cowpath + "/" + cowfile + ".cow");
    if (!ifs) {
        std::cout << exname << ": Could not find " << cowfile << 
                " cowfile!" << std::endl;
        return;
    }

    std::cout << " ";
    if (message.size() < wrap) {
        for(int i = 0; i < message.size() + 2; i++)
            std::cout << "-";
        std::cout << std::endl << "< " << message << " >" << std::endl << " ";
        for(int i = 0; i < message.size() + 2; i++)
            std::cout << "-";
    } else {
        unsigned int max_len = 0;
        auto v = break_down_message();
        for(auto l : v)
            if (l.size() > max_len)
                max_len = l.size();
        
        if (v.size() == 2) {
            for(int i = 0; i < max_len + 2; i++)
                std::cout << "-";
            std::cout << std::endl << "/ " << v[0] << std::endl;
            std::cout << "\\ " << v[1] << std::endl;
            std::cout << " ";
            for(int i = 0; i < max_len + 2; i++)
                std::cout << "-";
        } else {
            for(int i = 0; i < max_len + 2; i++)
                std::cout << "-";
            std::cout << std::endl;
            for(int index = 0; index < v.size(); index++) {
                if (index == 0) std::cout << "/ ";
                else if (index == v.size()-1) std::cout << "\\ ";
                else std::cout << "| ";
                
                std::cout << v[index];
                
                print_padding(v[index]);
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
            {"$thoughts", "\\"},
            {"$eyes", eyes},
            {"$tongue", tongue} };

    std::string out_str;
    while (!ifs.eof()) {
        std::getline(ifs, out_str);
        if (out_str.substr(0,2).compare("##") == 0)
            continue;
        if (out_str.find("EOC") == std::string::npos) {
            out_str = multi_replace(out_str, replace_list);
            std::cout << out_str << std::endl;
        }
    }
}

/* main */
int main(int argc, const char *argv[])
{
    int opt;

    while ((opt = getopt (argc, (char **)argv, "hlne:f:T:W:bdgpstwy")) != -1)
        switch (opt) {
            case 'h':
                display_usage(argv[0]);
                return EXIT_SUCCESS;
            case 'l':
                std::cout << "Cow files in " << 
                        cowpath << ":" << std::endl;
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
                tongue = "U";
                break;
            case 'g':
                eyes = "$$";
                break;
            case 'p':
                eyes = "@@";
                break;
            case 's':
                eyes = "**";
                tongue = "U";
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

        for (int index = optind; index < argc; index++) {
            add_word_to_message( argv[index]);
        }

        if (message_word_count==0)
            get_interactive_message(); 
        display_cow(argv[0]);
    return EXIT_SUCCESS;
}

