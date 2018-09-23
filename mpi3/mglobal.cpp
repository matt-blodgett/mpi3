#include "mglobal.h"


void Mpi3::register_global_metatypes(){
#ifdef Q_NAMESPACE
    qRegisterMetaType<ElementType>();
    qRegisterMetaType<MediaState>();
    qRegisterMetaType<EngineState>();
    qRegisterMetaType<ErrorState>();
#endif
}
