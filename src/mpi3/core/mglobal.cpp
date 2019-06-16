#include "mglobal.h"


namespace Mpi3
{
    void initialize()
    {
        qRegisterMetaType<ElementType>();
        qRegisterMetaType<MediaState>();
        qRegisterMetaType<EngineState>();
        qRegisterMetaType<ErrorState>();
    }
};

