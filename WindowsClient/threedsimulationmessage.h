#ifndef THREEDSIMULATIONMESSAGE_H
#define THREEDSIMULATIONMESSAGE_H


class ThreeDSimulationMessage
{
public:
    ThreeDSimulationMessage();
    int ballX=9999;
    int ballY=9999;
    int motorXangle=9999;
    int motorYangle=9999;
    int readyData=-1;
};

#endif // THREEDSIMULATIONMESSAGE_H
