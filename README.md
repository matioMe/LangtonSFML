# LangtonSFML

### Description:
A simple command line soft to generate colorful Langton ant drawings using the SFML library

### Installation:
To install the SFML library, follow the tutorial on this link:
https://www.sfml-dev.org/tutorials/2.4/

To compile with the terminal on Linux, you can open a terminal in the folder of the *langton.cpp* file and enter this command line:


    g++ -c langton.cpp && g++ langton.o -o langton.app -lsfml-graphics -lsfml-app -lsfml-system

To lauch the app, you can type:

    ./langton.app
