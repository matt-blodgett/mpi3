#include "mglobal.h"

void Mpi3::register_global_metatypes(){
    qRegisterMetaType<MediaState>();
    qRegisterMetaType<EngineState>();
    qRegisterMetaType<ErrorState>();
}
