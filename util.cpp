#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

void msleep(float ms)
{
    int us = ms*1000; // micro secs
    usleep(us);
}