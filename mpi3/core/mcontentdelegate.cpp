#include "mcontentdelegate.h"
#include "maudioengine.h"


MContentDelegate::MContentDelegate(QObject *parent) : QObject(parent){

}
MContentDelegate::~MContentDelegate(){}

void MContentDelegate::setEngine(MAudioEngine *engine){
    m_audioEngine = engine;
}
//void MContentDelegate::setContent(const QStringList &filepaths){
//    m_mediaContent = filepaths;
//}
void MContentDelegate::setIndex(int idx){
    QString queuePath = pathAt(idx);

    if(!queuePath.isNull()){
        m_index = idx;

        m_audioEngine->stop();
        m_audioEngine->open(queuePath);
        m_audioEngine->start();

        emit notifyIndex(idx);
    }
}

MAudioEngine* MContentDelegate::engine() const {
    return m_audioEngine;
}
QStringList MContentDelegate::content() const {
    return m_mediaContent;
}
QString MContentDelegate::pathAt(int idx) const {
    if(idx > 0 && idx < m_mediaContent.size()){
        return m_mediaContent.at(idx);
    }
    return QString();
}
int MContentDelegate::index() const {
    return m_index;
}

void MContentDelegate::processMediaStatus(Mpi3::MediaState state){
    Q_UNUSED(state)
}
void MContentDelegate::processEngineStatus(Mpi3::EngineState state){
    if(state == Mpi3::EngineStopped){
        setIndex(m_index + 1);
    }
}
void MContentDelegate::processErrorStatus(Mpi3::ErrorState state){
    Q_UNUSED(state)
}
void MContentDelegate::processRequestStatus(Mpi3::EngineState state){
    if(state == Mpi3::EngineActive && m_audioEngine->empty()){
        QString path = pathAt(m_index);

        if(!path.isNull()){
            m_audioEngine->open(path);
            m_audioEngine->start();
        }
    }
}
