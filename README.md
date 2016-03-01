cxxsay
======

`cxxsay` is a cowsay clone designed to handle gracefully unicode characters (UTF-8) as well as the ANSI escape codes used to color text in terminal emulators.

Dependencies
------------

The only thing needed is a C++ compiler able to handle C++11. A sufficently recent version of g++ or clang should be good.

Building
--------

Once the project clonned, you can run the following command to buil the project:

	make

Installing 
----------

Run the following command as root:

	make install

Uninstalling
------------

Run the following command as root:

    make uninstall

Usage
-----

`cxxsay` can be used pretty much like the original `cowsay`. All the behaviour of the original are reimplemented in this clone.

License
-------

Cowsay is under the WTFPL. See [LICENSE](https://raw.githubusercontent.com/Marneus68/cxxsay/master/LICENSE) for more details.
