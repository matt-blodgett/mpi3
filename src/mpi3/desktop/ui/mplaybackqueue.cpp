#include "mplaybackqueue.h"


MPlaybackQueue::MPlaybackQueue(QObject *parent) : QObject(parent)
{

}

//void MRootDesktop::processAudioMediaStatus(Mpi3::MediaState state)
//{
//    Q_UNUSED(state)
//}
//void MRootDesktop::processAudioEngineStatus(Mpi3::EngineState state)
//{
//    Q_UNUSED(state)
//}
//void MRootDesktop::processAudioErrorStatus(Mpi3::ErrorState state)
//{
//    Q_UNUSED(state)
//}
//void MRootDesktop::processAudioRequestStatus(Mpi3::EngineState state)
//{
//    Q_UNUSED(state)
////    if(state == Mpi3::EngineActive && m_audioEngine->empty()) {
////        if(m_treeSonglist->model()->rowCount() > 0) {
////            QModelIndex idx = m_treeSonglist->modelSortFilter()->index(0, 0);
////            m_treeSonglist->playItem(m_treeSonglist->modelSortFilter()->mapToSource(idx));
////        }
////    }
//}


//private:
//    void processAudioMediaStatus(Mpi3::MediaState state);
//    void processAudioEngineStatus(Mpi3::EngineState state);
//    void processAudioErrorStatus(Mpi3::ErrorState state);
//    void processAudioRequestStatus(Mpi3::EngineState state);
