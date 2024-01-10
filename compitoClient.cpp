#include <iostream>

#include "UDP.hpp"
#include "compitoStruct.h"

#define TIMEOUT	1000 // 1 secondo

using namespace std;

int main(int argc, char* argv[])
{
	uint32_t ip_address;
	uint16_t port_number;
	int n;

	if (argc < 3)
	{
		cout << "Uso: " << argv[0] << " indirizzo-IP codice e 0/1" << endl;
		return -1;
	}

	UDP socket(0);
	ip_address = dotted2binary(argv[1]);
	port_number = 12345;
    CMD cmd;
    ANS ans;

    strncpy(cmd.codice, argv[2], 16);
    int c= (int)atoi(argv[3]);


	switch (c)
	{

		case '0':
					cmd.cmd = VER;
					break;
		case '1':
					cmd.cmd = EXT;
					break;

		default:	cout << "Errore" << endl;
					return -1;
	};

    socket.send(ip_address, port_number, (uint8*)(&cmd), sizeof(CMD)); //invio la struttura al server


	if (socket.wait(TIMEOUT) > 0)
	{
		if ((n = socket.receive(&ip_address, &port_number, (uint8_t*)(&ans), sizeof(ANS))) == sizeof(ANS))
		{

			switch (ans.cmd)
			{
				case VER:
					if (ans.err == ERROR){
                        cout << "errore" << endl;
                    }
                    if (ans.err== NONVALIDO){
                        cout << "non valido"<<endl;
                    }
					else{
						cout << "corretto" << endl;
                    }
					ans.cmd = VER; break;

				case EXT:
							if (ans.err==OK){
								cout << "OK!" << endl;
                            }
							else{
								cout << "ERRORE!" << endl;
                            }
							break;
				default:
							cout << "Errore!" << endl;
							break;
			}
			return 0;
		}
	}

	cout << "risposta non ricevuta!" << endl;
	return -1;
}
