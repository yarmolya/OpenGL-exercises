//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================


#ifdef _WIN32
#  include <windows.h>
#  include <stdlib.h>
#  include <errhandlingapi.h>
#endif

#include "solar_viewer.hh"

//=============================================================================


int main(int argc, char *argv[])
{
#ifdef _WIN32
    // This make crashes very visible - without them, starting the
    // application from cmd.exe or powershell can surprisingly hide
    // any signs of a an application crash!
    SetErrorMode(0);
#endif
    try {
        Solar_viewer window("Solar System", 640, 480);
        return window.run();
    } catch (std::runtime_error const &e) {
        std::cerr << "FATAL ERROR: "
            << e.what()
            << std::endl;
        return 1;
    }
}


//=============================================================================
