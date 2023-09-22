// Simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>

#include "fmi3Functions.h"
#include "fmi3FunctionTypes.h"
#include "fmi3PlatformTypes.h"

#include <boost/dll/import.hpp>

class FMI3Interface{
public:
    fmi3InstantiateCoSimulationTYPE* pFmuInstantiateCoSimulation;
    fmi3FreeInstanceTYPE* pFmuFreeInstance;
    fmi3EnterInitializationModeTYPE* pFmuEnterInitializationMode;
    fmi3ExitInitializationModeTYPE* pFmuExitInitializationMode;
    fmi3TerminateTYPE* pFmuTerminate;
    fmi3ResetTYPE* pFmuReset;
    fmi3EnterEventModeTYPE* pFmuEnterEventMode;
    fmi3DoStepTYPE* pFmuDoStep;
    fmi3UpdateDiscreteStatesTYPE* pFmuUpdateDiscreteStates;
    fmi3EnterStepModeTYPE* pfmuEnterStepMode;

    fmi3SetBooleanTYPE* pFmuSetBoolean;

    fmi3SetFloat32TYPE* pFmuSetFloat32;
    fmi3SetFloat64TYPE* pFmuSetFloat64;

    fmi3SetInt8TYPE* pFmuSetInt8;
    fmi3SetInt16TYPE* pFmuSetInt16;
    fmi3SetInt32TYPE* pFmuSetInt32;
    fmi3SetInt64TYPE* pFmuSetInt64;

    fmi3GetBooleanTYPE* pFmuGetBoolean;

    fmi3GetFloat32TYPE* pFmuGetFloat32;
    fmi3GetFloat64TYPE* pFmuGetFloat64;

    fmi3GetInt8TYPE* pFmuGetInt8;
    fmi3GetInt16TYPE* pFmuGetInt16;
    fmi3GetInt32TYPE* pFmuGetInt32;
    fmi3GetInt64TYPE* pFmuGetInt64;
};

void simulation(FMI3Interface& fmiInterface){
    auto instance = fmiInterface.pFmuInstantiateCoSimulation("Stair", "{BD403596-3166-4232-ABC2-132BDF73E644}", NULL, fmi3False, fmi3False, fmi3True, fmi3True, NULL, 0, NULL, NULL, NULL);
    if (instance == nullptr) {
        throw;
    }

    auto status = fmiInterface.pFmuEnterInitializationMode(instance, fmi3False, 0.0, 0, fmi3True, 10);
    status = fmiInterface.pFmuExitInitializationMode(instance);

    fmi3Boolean discreteStatesNeedUpdate = false;
    fmi3Float64 step = 0.0;
    
    fmi3Boolean eventHandlingNeeded = false;
    fmi3Boolean terminateSimulation = false;
    fmi3Boolean earlyReturn = false;
    fmi3Float64 lastSuccessfulTime = 0.0;

    fmi3Boolean nominalsOfContinuousStatesChanged = false;
    fmi3Boolean valuesOfContinuousStatesChanged = false;
    fmi3Boolean nextEventTimeDefined = false;
    fmi3Float64 nextEventTime = 0.0;

    do {
        status = fmiInterface.pFmuUpdateDiscreteStates(instance, &discreteStatesNeedUpdate, &terminateSimulation, &nominalsOfContinuousStatesChanged, &valuesOfContinuousStatesChanged, &nextEventTimeDefined, &nextEventTime);
        if (terminateSimulation) {
            break;
        }
    } while (discreteStatesNeedUpdate);

    status = fmiInterface.pfmuEnterStepMode(instance);

    while (1) {
        //if (terminateSimulation || time >= stopTime) {
        //    break;
        //}

        status = fmiInterface.pFmuDoStep(instance, step, 0.1, fmi3True, &eventHandlingNeeded, &terminateSimulation, &earlyReturn, &lastSuccessfulTime);
        if (eventHandlingNeeded || nextEventTimeDefined && std::abs(step - nextEventTime) < 0.01) {
            status = fmiInterface.pFmuEnterEventMode(instance);

            int a = 10;

            do {
                status = fmiInterface.pFmuUpdateDiscreteStates(instance, &discreteStatesNeedUpdate, &terminateSimulation, &nominalsOfContinuousStatesChanged, &valuesOfContinuousStatesChanged, &nextEventTimeDefined, &nextEventTime);
            } while (discreteStatesNeedUpdate);

            status = fmiInterface.pfmuEnterStepMode(instance);
            if (terminateSimulation) {
                break;
            }
        }
        step += 1;
    }
}

int main()
{
    using namespace boost::dll;
    shared_library dll("E:\\uni-stuttgart\\subjects\\5_ws23\\MasterThesis\\Reference-FMUs\\out\\build\\x64-Debug\\temp\\Stair\\binaries\\x86_64-windows\\Stair.dll");

    if (!dll.is_loaded()) {
        std::cout << "Dll load failed \n";
        return EXIT_FAILURE;
    }

    FMI3Interface fmiInterface;

    fmiInterface.pFmuInstantiateCoSimulation = dll.get<fmi3InstantiateCoSimulationTYPE>("fmi3InstantiateCoSimulation");
    fmiInterface.pFmuFreeInstance = dll.get<fmi3FreeInstanceTYPE>("fmi3FreeInstance");
    fmiInterface.pFmuEnterInitializationMode = dll.get<fmi3EnterInitializationModeTYPE>("fmi3EnterInitializationMode");
    fmiInterface.pFmuExitInitializationMode = dll.get<fmi3ExitInitializationModeTYPE>("fmi3ExitInitializationMode");
    fmiInterface.pFmuTerminate = dll.get<fmi3TerminateTYPE>("fmi3Terminate");
    fmiInterface.pFmuReset = dll.get<fmi3ResetTYPE>("fmi3Reset");
    fmiInterface.pFmuEnterEventMode = dll.get<fmi3EnterEventModeTYPE>("fmi3EnterEventMode");
    fmiInterface.pFmuDoStep = dll.get<fmi3DoStepTYPE>("fmi3DoStep");

    fmiInterface.pFmuUpdateDiscreteStates = dll.get<fmi3UpdateDiscreteStatesTYPE>("fmi3UpdateDiscreteStates");
    fmiInterface.pfmuEnterStepMode = dll.get<fmi3EnterStepModeTYPE>("fmi3EnterStepMode");

    fmiInterface.pFmuSetBoolean = dll.get<fmi3SetBooleanTYPE>("fmi3SetBoolean");

    fmiInterface.pFmuSetFloat32 = dll.get<fmi3SetFloat32TYPE>("fmi3SetFloat32");
    fmiInterface.pFmuSetFloat64 = dll.get<fmi3SetFloat64TYPE>("fmi3SetFloat64");

    fmiInterface.pFmuSetInt8 = dll.get<fmi3SetInt8TYPE>("fmi3SetInt8");
    fmiInterface.pFmuSetInt16 = dll.get<fmi3SetInt16TYPE>("fmi3SetInt16");
    fmiInterface.pFmuSetInt32 = dll.get<fmi3SetInt32TYPE>("fmi3SetInt32");
    fmiInterface.pFmuSetInt64 = dll.get<fmi3SetInt64TYPE>("fmi3SetInt64");

    fmiInterface.pFmuGetBoolean = dll.get<fmi3GetBooleanTYPE>("fmi3GetBoolean");
   
    fmiInterface.pFmuGetFloat32 = dll.get<fmi3GetFloat32TYPE>("fmi3GetFloat32");
    fmiInterface.pFmuGetFloat64 = dll.get<fmi3GetFloat64TYPE>("fmi3GetFloat64");

    fmiInterface.pFmuGetInt8 = dll.get<fmi3GetInt8TYPE>("fmi3GetInt8");
    fmiInterface.pFmuGetInt16 = dll.get<fmi3GetInt16TYPE>("fmi3GetInt16");
    fmiInterface.pFmuGetInt32 = dll.get<fmi3GetInt32TYPE>("fmi3GetInt32");
    fmiInterface.pFmuGetInt64 = dll.get<fmi3GetInt64TYPE>("fmi3GetInt64");
    
    simulation(fmiInterface);

    dll.unload();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
