#include <stdio.h>
#include <stdlib.h>
#include "calc.nsmap"
#include "soapH.h"

#define URL "http://localhost:2000"

int main(int argc, char** argv)
{
    struct soap soap;
    double a, b, sum;

    soap_init1(&soap, SOAP_XML_INDENT);

    while(1) {
        a = random() % 1000;
        b = random() % 1000;
        soap_call_calc__add(&soap, URL, "", a, b, &sum);
        if (soap.error) {
            soap_print_fault(&soap, stderr);
        } else {
            printf("%.1f + %.1f == %.1f\n", a, b, sum);
        }

        soap_destroy(&soap);
        soap_end(&soap);
    }

    return 0;
}
