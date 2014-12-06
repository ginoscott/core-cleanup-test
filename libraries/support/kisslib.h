#ifndef KISSLIB
#define KISSLIB

#include "configCosmos.h"

#include <cstring>
#include <iostream>

using namespace std;

#define FEND  0xC0
#define FESC  0xDB
#define TFEND 0xDC
#define TFESC 0xDD

#define PACKETMAX 1024


class kissHandle
{

public:
	kissHandle(); // default constructor
	kissHandle( 		int port,
				int comm,
				const char dest_call[],
				char dest_stat,
				const char sour_call[],
				char sour_stat,
				char cont,
				char prot);

	void set_port_number(unsigned int P);
	unsigned int get_port_number();
	void set_command(unsigned int C);
	unsigned int get_command();
	void set_destination_callsign(const char destination[]);
	unsigned char* get_destination_callsign();	
	void set_destination_stationID(unsigned int ID);
	unsigned int get_destination_stationID();
	void set_source_callsign(const char source[]);
	unsigned char* get_source_callsign();	
	void set_source_stationID(unsigned int ID);
	unsigned int get_source_stationID();
	void set_control(unsigned int control_number);
	unsigned int get_control();
	void set_protocolID(unsigned int protocol);
	unsigned int get_protocolID();
	
private:
	unsigned int  port_number;
	unsigned int  command;
	unsigned char destination_callsign[6];
	unsigned char destination_stationID;
	unsigned char source_callsign[6];
	unsigned char source_stationID;
	unsigned char control;
	unsigned char protocolID;

	friend ostream& operator<<(ostream& out, kissHandle& K);
};

//old encode (to be deleted)**********************************************
int kissEncode(uint8_t *input, uint32_t count, uint8_t *encoded_packet);
//new encode
int kissEncode(uint8_t *input, uint32_t count, uint8_t *encoded_packet, kissHandle* handle);

//int kissDecode(uint8_t *input, uint32_t count, uint8_t *decoded_packet);

int kissDecode(uint8_t* kissed_input, uint32_t count, uint8_t* decoded_payload);

kissHandle kissInspect(const unsigned char* input);

//Functions to print packets
void print_ascii(unsigned char* packet, unsigned int count);
void print_hex(unsigned char* packet, unsigned int count);

#endif
