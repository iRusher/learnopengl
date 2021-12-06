//
// Created by Rusher on 2021/12/6.
//

#include "Log.h"

#include <string>
#include <vector>

const std::vector<std::string> LOG_LEVEL_DESCS{"ERROR", "WARN", "INFO", "DEBUG"};

void Log::logMessage(LogLevel level, const char *format, ...) {

    char  buff[4096];
    char *p    = buff;
    char *last = buff + sizeof(buff) - 3;

    p += sprintf(p, "[%s]: ", LOG_LEVEL_DESCS[static_cast<int>(level)].c_str());

    va_list args;
    va_start(args, format);

    ptrdiff_t count = (last - p);
    int       ret   = vsnprintf(p, count, format, args);
    if (ret >= count - 1) {
        p += (count - 1);
    } else if (ret >= 0) {
        p += ret;
    }

    va_end(args);

    *p++ = '\n';
    *p   = 0;

    fputs(buff, stdout);

}
