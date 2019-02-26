#include "mglobal.h"


void Mpi3::initialize()
{
    qRegisterMetaType<ElementType>();
    qRegisterMetaType<MediaState>();
    qRegisterMetaType<EngineState>();
    qRegisterMetaType<ErrorState>();
}
