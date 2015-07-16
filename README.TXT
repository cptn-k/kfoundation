**********************
* KFoundation v1.0.2 *
**********************


Contents
--------

  1. About
  2. License
  3. System Requirements
  4. How to Download
  5. How to Build
  6. How to Use
  7. Where to Find Additional Resources
  8. How to Request a Feature or Report a Bug
  9. Credits



1. About
--------

  KFoundation is a general-purposed C++ library. It contains a variety of
different tools and facilities for memory management, I/O, logging and more.
The purpose of this project is to have a more useful, efficient, and elegant
replacement for C++ standard libraries, maintaining portability to targets that
are still not supported by newer compilers.

  With release of every new version of C++, the API of standard libraries gets
more complicated along with the language syntax. Yet, there are standard
libraries like that of Java language, or Foundation Framework of Objective-C
language that are more versatile and useful, yet they are more elegant even
though they do not enjoy some of the language features available to C++
programs. KFoundtion projects brings missing features and missing elegant back
to C++.

Featrues:

  - Automatic Memory Management
    - Safe: Detects all access violations, but throws exception instead of
      segmentation fault.
    - Easy: Type "Ptr<MyClass> x = new MyClass()" and the rest is automatic.
    - Fast: All algorithms run in O(1) time complexity --- the fastest possible.
    - Compatible down to C++98
    - Extensible with custom memory managers
    - Supplemented with managed containers

  - Exceptions
    - Human readable stack trace
    - Debugging made easy
    - Serializable (to XML, etc.) for remote debugging

  - Object Serialization/Deserialization
    - Back and forth to XML, JSON, and KFOR (KFoundation Format)
    - Extensible with more formats
    - Intuitive API can be used for every object
    - Almost all standard KFoundation classes are serializable
      (include exceptions)
    - Serializable objects can be directly fed into logger
    - Ready for RESTful applications

  - IO Stream
    - Minimalistic API
    - Wider variety of streams including buffer, string, and net I/O
    - Supplemented with versatile parser utility

  - Logger
    - Multi-level
    - Multi-channel
    - Customizable
    - Easy: LOG << "My value: " << 12 << EL;

  - System class
    - Cross-platform, Object-oriented way to access system features

  - Cross-platform multi-threading



2. License
----------

  KFoundaion is protected under KNORBA FREE PUBLIC LICENSE. You are allowed to
use, modify, or redistribute the source code, but prohibited to use it for
military, spying, terrorism, and other harmful applications, or to provide it
to another person having the intention for it to be used as such.

Please refer to the accompanying LICENSE.TXT file for details.


3. System Requirements
----------------------

  - CMake 2.6 or above
  - Any C++98 or above compiler compatible with CMake


4. How to Download
------------------

  Either clone the git repository:

    $git clone https:://github.com/hkhandan/kfoundation.git

or, visit

    https://github.com/hkhandan/kfoundation/releases

to download the software as a compressed archive.



5. How to Build
---------------

  Assuming the source code is in "kfoundation" directory,

    $cd kfoundation
    $mkdir build
    $cd build
    $cmake ..
    $make
    $sudo make install

  You may want to use ccmake before make to change the installation directory.



6. How to Use
-------------

  To use you need to link your code against

    INSTALL_DIR/lib/kfoundation.a

and add

    INSTALL_DIR/include

to your include directories.

  Each KFoundation header file is named the same as the class it declares. For
example, to use System class,

    #include <kfoundation/System.h>

and add

    using namespace kfoundation;



7. Where to Find Additional Resources
-------------------------------------

  Please checkout

    http://hkhandan.github.io/kfoundation/

  for latest info.



8. How to Request a Feature or Report a Bug
-------------------------------------------

  Your beedback and ideas are more than welcome. If you have a github account,
the preffered way is via github:

    https://github.com/hkhandan/kfoundation/issues

  Otherwise, please e-mail the authors (below).



9. Credits
----------

  Hamed "Kay" KHANDAN, Dr. Eng.

    Affiliation1: Graduate School of Systems Informatics, Kobe University
    Affiliation2: AICS Advanced Institute for Computational Science
    E-mail: hkhandan@yahoo.com
