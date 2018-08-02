/*
 * Bergamota-ng Command line interface (c) 2018 Cassiano Martin <cassiano@polaco.pro.br>
 * Copyright (c) 2018 Cassiano Martin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <signal.h>

#include "main.h"

jmp_buf rpoint;

void sig_handler(int sig)
{
    if(sig == SIGSEGV)
    {
        printf("Received segfault\n");
        signal(SIGSEGV, &sig_handler);
        longjmp(rpoint, SIGSEGV);
    }
}

int main(int argc, char **argv)
{
    signal(SIGSEGV, &sig_handler);

    int fcode = setjmp(rpoint);

    if(fcode == 0)
    {
        char *type = getenv("REQUEST_METHOD");
        char *url = getenv("REQUEST_URI");
        int lenght = atoi(getenv("CONTENT_LENGTH"));
        int i;

        char *buf = (char *)malloc(sizeof(char)*(lenght+1));

        for(i=0; i<lenght; i++)
            buf[i] = fgetc(stdin);
        buf[i]='\0';

        printf("Content-Type: application/json\r\n");
        printf("Accept: application/json\r\n");
        printf("\r\n");
        printf("{ \"type\": \"%s\", \"url\": \"%s\", \"lenght\": \"%d\", \"buf\": \"%s\" }", type, url, lenght, buf);

        free(buf);
    }
    else
    {
        //DEBUG("Application crashed, received a SIGSEGV code: %d", fcode);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}