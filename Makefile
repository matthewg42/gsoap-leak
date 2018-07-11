CC = gcc

SERVICE = calc

# where I built the source (stdsoap2.c found under here)
GSOAP_SRC = $(HOME)/project/gsoap-2.8.67

# where gsoap is installed (argument to --prefix when building)
GSOAP_DIR = /opt/gsoap-2.8.67

# commenting out this line fixes the leak!
CFLAGS += -DDEBUG 

# misc compiler and linker options
CFLAGS += -Wno-stringop-overflow
CFLAGS += -g

WSDL2H = $(GSOAP_DIR)/bin/wsdl2h
SOAPCPP2 = $(GSOAP_DIR)/bin/soapcpp2
CFLAGS += $(shell PKG_CONFIG_PATH="$(GSOAP_DIR)/lib/pkgconfig" pkg-config --cflags gsoap)
LFLAGS += $(shell PKG_CONFIG_PATH="$(GSOAP_DIR)/lib/pkgconfig" pkg-config --libs gsoap)


TARGETS = client_calc server_calc
GENFILES = $(SERVICE).nsmap soapStub.h soapServer.c soapH.h soapC.c soapClient.c

.PHONY : clean all gen scrub

all : $(TARGETS) 

gen : $(SERVICE).h
	$(SOAPCPP2) -c -L -g $<
	ln -s $(GSOAP_SRC)/gsoap/stdsoap2.c ./

$(SERVICE).h : $(SERVICE).wsdl
	$(WSDL2H) -c -t typemap.dat -o $@ $<

%.o : %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

client_calc : client.o soapC.o soapClient.o stdsoap2.o 
	$(CC) -o $@ $(LFLAGS) $^

server_calc : server.o soapC.o soapServer.o stdsoap2.o 
	$(CC) -o $@ $(LFLAGS) $^ -lm

clean : 
	rm -f $(TARGETS) *.o

scrub : clean
	rm -f $(SERVICE).h $(GENFILES) $(SERVICE).*.req.xml $(SERVICE).*.res.xml {TEST,SEND,RECV}.log stdsoap2.c *.log

