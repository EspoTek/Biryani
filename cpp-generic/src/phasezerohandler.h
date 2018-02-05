#ifndef PHASEZEROHANDLER_H
#define PHASEZEROHANDLER_H

#include <vector>
#include <iostream>

class initTransfer;
class usbInterface;

class phaseZeroHandler
{
public:
    phaseZeroHandler();
    int checkIfAlreadyInitialised();
    int createInitPattern(void);
    int sendInitPattern(void);
private:
    std::vector<initTransfer*> initPattern;
    usbInterface * phase0_interface;
};

#endif // PHASEZEROHANDLER_H
