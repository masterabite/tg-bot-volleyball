#include <iostream>

#include <MASBot.h>


/*
    BUILD_MODE:
        0   -   develop
        1   -   debug
        2   -   production
*/
#ifndef BUILD_MODE
#define BUILD_MODE 0
#endif

int main() {
    
    std::string botToken;

    #if BUILD_MODE < 2     
    botToken = "7298813441:AAERfbqWxHg7IYLEDn-PAlQjHVyv7gCoUDA";
    #else
    botToken = "6600829958:AAGoFFj1EmI6WiYjpXloM-7PSrNezSyO6VQ";
    #endif
    
    MASBot masBot(botToken);
    return 0;
}

