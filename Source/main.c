#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

#include <vpad/input.h>
#include <whb/log_console.h>
#include <whb/log.h>
#include <whb/log_udp.h>
#include <whb/proc.h>

int main(int argc, char** argv)
{
	//Init stuff
    
    WHBProcInit();
	WHBLogConsoleInit();
    WHBLogUdpInit();
    WHBLogConsoleSetColor(0x00FFF000);

    //Text on Screen

	WHBLogPrint("Rumble Tester Made by Fangal :3");
    WHBLogPrint("Press and hold A to rumble");
	WHBLogConsoleDraw();

    //Decs

	VPADStatus status;
	VPADReadError error;
	bool vpad_fatal = false;
    uint8_t pattern = 0xFF;
    uint8_t length = 1;

	while (WHBProcIsRunning())
	{       
        //DRC reading and errors
        
        VPADRead(VPAD_CHAN_0, &status, 1, &error);

        switch (error) {
        case VPAD_READ_SUCCESS: {
            break;
        }
        case VPAD_READ_NO_SAMPLES: {
            continue;
        }
        case VPAD_READ_INVALID_CONTROLLER: {
            WHBLogPrint("Gamepad disconnected!");
            vpad_fatal = true;
            break;
        }

        default: {
            WHBLogPrintf("Unknown VPAD error! %08X", error);
            vpad_fatal = true;
            break;
        }
        }

        if (vpad_fatal)
        {
            WHBLogPrint("For some reason gamepad messed up");
            break;
        }

        //Inputs and rumbling

        if (status.hold & VPAD_BUTTON_A) 
        {
            WHBLogPrint("Pressed A, starting rumble");
            VPADControlMotor(VPAD_CHAN_0, &pattern, length);
        }
        else if (status.release & VPAD_BUTTON_A) 
        {
            WHBLogPrint("Released A, stopping rumble");
            VPADStopMotor(VPAD_CHAN_0);
        }
	}

    //Quit Stuff

    WHBLogPrint("Quitting");
    WHBLogConsoleFree();
    WHBLogUdpDeinit();
    WHBProcShutdown();

    return 0;
}