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



//void MTreeSonglist::populateQueue()
//{
//    m_playbackQueue.clear();

//    for(int i = 0; i < m_modelSortFilter->rowCount(); i++) {
//        QModelIndex idx_sort = m_modelSortFilter->index(i, 0);
//        QModelIndex idx_source = m_modelSortFilter->mapToSource(idx_sort);
//        m_playbackQueue.append(m_modelSonglist->songAt(idx_source));
//    }
//}
//void MTreeSonglist::sortChanged(int logicalIndex, Qt::SortOrder order)
//{
//    Q_UNUSED(logicalIndex)
//    Q_UNUSED(order)

//    if(m_modelSonglist->container() == m_playbackContainer) {
//        populateQueue();
//    }
//}

//void MTreeSonglist::playItem(const QModelIndex &idx)
//{
//    MSong *song = m_modelSonglist->songAt(idx);

//    m_playbackContainer = m_modelSonglist->container();
//    m_playbackSong = song;

//    populateQueue();

//    QItemSelectionModel::SelectionFlags flag;
//    flag = QItemSelectionModel::ClearAndSelect;
//    flag |= QItemSelectionModel::Rows;
//    selectionModel()->select(idx, flag);

//    emit playbackChanged(song);
//}
//void MTreeSonglist::shiftItem(int offset)
//{
//    if(m_playbackQueue.size() == 0) {
//        return;
//    }

//    int idx_song = m_playbackQueue.indexOf(m_playbackSong);
//    idx_song += offset;

//    if(idx_song < m_playbackQueue.size() && idx_song >= 0) {

//        MSong *song = m_playbackQueue[idx_song];
//        m_playbackSong = song;

//        emit playbackChanged(song);

//        if(m_modelSonglist->container() == m_playbackContainer) {

//            QItemSelectionModel::SelectionFlags flag;

//            flag = QItemSelectionModel::Rows;
//            flag |= QItemSelectionModel::ClearAndSelect;

//            QModelIndex idx_select = m_modelSortFilter->index(idx_song, 0);
//            selectionModel()->select(idx_select, flag);
//        }
//    }
//}

//void MTreeSonglist::playNextItem()
//{
//    shiftItem(1);
//}
//void MTreeSonglist::playPrevItem()
//{
//    shiftItem(-1);
//}
