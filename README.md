cxxsay
======

*cxxsay* is a [cowsay]() clone designed to handle gracefully UTF-8 as well as the ANSI escape codes used to color text in terminal emulators.

Features
--------

Unlike the original *cowsay*, *cxxsay* is build to handle UTF-8 characters gracefully:

![]()

It also handles ANSI escape sequences properly:

![]()

Just like *cowsay*, *cxxsay* comes with a "think" alias.

Usage
-----

Usage is the same as the original cowsay.

The `-h` argument or any invalid command will trigger the usage message.

    cxx{say,think} version 0.1, (c) 2014 Duane Bekaert
    Usage: ./cxxsay [-bdgpstwy] [-h] [-e eyes] [-f cowfile] 
          [-l] [-n] [-T tongue] [-W wrapcolumn] [message]


Installing
----------

### Binaries

#### Mac OS X

`TODO`

#### Linux

`TODO`

### From source

The only thing needed to build *cxxsay* from source is a C++ compiler able to handle C++11. A sufficiently recent version of g++ or clang should be good enough.

Clone the repository:

    git clone https://github.com/Marneus68/cxxsay.git

You can both build and install everything by running the following as root:

    make install

Uninstalling
------------

Run the following command as root:

    make uninstall

License
-------

*cxxsay* is under the WTFPL. See [LICENSE](https://raw.githubusercontent.com/Marneus68/cxxsay/master/LICENSE) for more details.
