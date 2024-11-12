#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>

#define LOG_FILE "server.log"

// Function to get the current time as a string
static const char* get_current_time() {
    static char buffer[20];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
}

// Function to log a message both to a file and console
void log_message(const char* level, const char* fmt, ...) {
    va_list args;
    FILE *log_file = fopen(LOG_FILE, "a");  // Open the log file in append mode

    if (log_file == NULL) {
        perror("Unable to open log file");
        return;
    }

    // Get the current time
    const char* current_time = get_current_time();

    // Print the message to the console
    printf("[%s] [%s] ", current_time, level);
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");

    // Write the message to the log file
    fprintf(log_file, "[%s] [%s] ", current_time, level);
    va_start(args, fmt);
    vfprintf(log_file, fmt, args);
    va_end(args);
    fprintf(log_file, "\n");

    fclose(log_file);
}