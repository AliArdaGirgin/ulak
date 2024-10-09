#ifndef CONF_H
#define CONF_H

#include <QtSystemDetection>

#if  defined(Q_OS_LINUX)
    #define RCC_FILE "./img/resources.rcc"
    #define STYLESHEET_FILE "./stylesheet.css"
#elif defined(Q_OS_WIN)
    #define RCC_FILE "..\\..\\img\\resources.rcc"
    #define STYLESHEET_FILE "..\\..\\stylesheet.css"
#else
    #error "Operatin system not supported"
#endif

#define COMMAND_AREA_TIMER_RESOLUTION 100 //ms
#define DATA_AREA_TIMER_RESOLUTION 100 //ms

#endif // CONF_H
