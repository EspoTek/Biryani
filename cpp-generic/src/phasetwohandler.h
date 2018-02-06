#ifndef PHASETWOHANDLER_H
#define PHASETWOHANDLER_H

#include <chrono>
#include <thread>
#include <mutex>

class usbInterface;

class phaseTwoHandler
{
public:
    phaseTwoHandler();
    void deleteThread();
    int enterPhaseTwo(int phase_2_packet_length, usbInterface *interface);
private:
    usbInterface *phase2_interface = NULL;
    std::thread *worker;
};

#endif // PHASETWOHANDLER_H
