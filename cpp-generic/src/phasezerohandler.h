#ifndef PHASEZEROHANDLER_H
#define PHASEZEROHANDLER_H

class initTransfer;

class phaseZeroHandler
{
public:
    phaseZeroHandler();
    int checkIfAlreadyInitialised();
    int createPreinitPattern(void){
private:
    std::vector<initTransfer*> preinitPattern;
};

#endif // PHASEZEROHANDLER_H
