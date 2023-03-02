/*
 * mc_server.cpp
 *
 * Created: 2/28/2023 3:25:23 PM
 *  Author: Jorge Guerra
 */ 
#include "mc_server.h"
#include "ClearCore.h"
#include "EthernetTcpServer.h"



constexpr uint32_t PORT_NUM = 8888;
constexpr uint32_t MAX_PACKET_LENGTH = 100;
constexpr uint32_t MAX_NUMBER_CLIENTS = 2; // Allow the supervisory controller and an external pc to connect to the ClearCore
constexpr uint32_t DEFAULT_IP_ADDRESS[4] = {192,168,1,20};// .20 is the first IP Address for the motor controllers
	

class ConnectedClients{
	private:
	EthernetTcpClient clients[MAX_NUMBER_CLIENTS];
	uint8_t clientCount = 0;
	public:
	const EthernetTcpClient& operator[](size_t idx)const{return clients[idx];}
	void add_client(EthernetTcpClient newClient){
		if(clientCount<MAX_NUMBER_CLIENTS){
			clients[clientCount].Close();
			clients[clientCount] = newClient;
			clientCount++;
		}else{
			newClient.Send("Server has reached its maximum number of clients");
			newClient.Close();
		}
	}
	
		
}

struct ClientMessage{
	char* ipAddress;
	char* message;
}
	
namespace {
	EthernetTcpServer server = EthernetTcpServer(PORT_NUM);
	ConnectedClients clients;
}
	
	

void mc_server_init(){
	while(!EthernetMgr.PhyLinkActive()){
		ConnectorUsb.SendLine("Ethernet cable is unplugged");
		Delay_ms(1000);
	}
	EthernetMgr.Setup();
	// TODO: Add method to ask user to enter IP address via serial, this should only 
	// be added at time of MFCT or by technician during commissioning or troubleshooting
	auto ip = IpAddress(DEFAULT_IP_ADDRESS[0],DEFAULT_IP_ADDRESS[1],DEFAULT_IP_ADDRESS[2],DEFAULT_IP_ADDRESS[3]);
	EthernetMgr.LocalIp(ip);
	ConnectorUsb.Send("Assigned manual IP address: ");
	ConnectorUsb.SendLine(EthernetMgr.LocalIp().StringValue());
	server.Begin();
}

void update_clients(){
	auto client = server.Accept();
	if(client.Connected()){
		clients.add_client(client);
	}
}

