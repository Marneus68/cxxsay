#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include <string>

#include "enum.h"

/* cxxsay specific functions */
void add_word_to_message(const std::string & word);
void get_interactive_message();
void print_padding(const std::string & line);
void display_usage(const std::string & exname);
void display_cow_list();
void display_cow(const std::string & exname);

int cowsay(int argc, const char *argv[], RunType type);

#endif /* __FUNS_H__ */
