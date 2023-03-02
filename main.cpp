
#include "ClearCore.h"

constexpr uint32_t TIMEOUT = 5000;

enum class SystemStatus{
	faulted,
	};

void initialize(){
	ConnectorUsb.Mode(Connector::USB_CDC);
	ConnectorUsb.Speed(9600);
	ConnectorUsb.PortOpen();
	uint32_t start_time = Milliseconds();
	//Initialize Ethernet communications and any other devices...
}


int main(void) {
	
    /* Replace with your application code */
    bool ledState = true;
    while (1)
    {
        ConnectorLed.State(ledState);
        ledState = !ledState;
        Delay_ms(1000);
    }
}