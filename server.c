/* adapted from the calc server sample in the gsoap-2.8.68 distribution */

#include "soapH.h"
#include "calc.nsmap"

#define PORT 2000

int main(int argc, char **argv)
{   
    SOAP_SOCKET m, s; /* sockets */
    struct soap soap;
    soap_init(&soap);
    m = soap_bind(&soap, NULL, PORT, 100);
    if (!soap_valid_socket(m))
    {   soap_print_fault(&soap, stderr);
        exit(1);
    }
    fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
    for ( ; ; )
    {   
        s = soap_accept(&soap);
        fprintf(stderr, "Socket connection successful: slave socket = %d\n", s);
        if (!soap_valid_socket(s))
        {   soap_print_fault(&soap, stderr);
            exit(1);
        }
        soap_serve(&soap);
        soap_end(&soap);
    }
    return 0;
}

int calc__add(struct soap *soap, double a, double b, double *result)
{   (void)soap;
    *result = a + b;
    return SOAP_OK;
}

int calc__sub(struct soap *soap, double a, double b, double *result)
{   (void)soap;
    *result = a - b;
    return SOAP_OK;
}

int calc__mul(struct soap *soap, double a, double b, double *result)
{   (void)soap;
    *result = a * b;
    return SOAP_OK;
}

int calc__div(struct soap *soap, double a, double b, double *result)
{   if (b)
        *result = a / b;
    else
    {   char *s = (char*)soap_malloc(soap, 1024);
        (SOAP_SNPRINTF(s, 1024, 100), "<error xmlns=\"http://tempuri.org/\">Can't divide %f by %f</error>", a, b);
        return soap_sender_fault(soap, "Division by zero", s);
    }
    return SOAP_OK;
}

int calc__pow(struct soap *soap, double a, double b, double *result)
{   *result = pow(a, b);
    if (soap_errno == EDOM)	/* soap_errno is like errno, but compatible with Win32 */
    {   char *s = (char*)soap_malloc(soap, 1024);
        (SOAP_SNPRINTF(s, 1024, 100), "<error xmlns=\"http://tempuri.org/\">Can't raise %f to %f</error>", a, b);
        return soap_sender_fault(soap, "Power function domain error", s);
    }
    return SOAP_OK;
}
