#include <iostream>
#include "../include/Logging.h"

using namespace std;


int main(void)
{
    Logger::setLogLevel(Logger::TRACE);

    LOG_INFO << "This is info";
    LOG_WARN << "This is warn";
    LOG_TRACE << "This Trace";
    LOG_DEBUG << "This Debug";
    return 0;
}
