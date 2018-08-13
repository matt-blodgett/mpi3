#include "mcontentdelegate.h"
#include "maudioengine.h"
#include "mmedialibrary.h"


#include <QDebug>


MContentDelegate::MContentDelegate(QObject *parent) : QObject(parent){}
MContentDelegate::~MContentDelegate(){}

void MContentDelegate::setEngine(MAudioEngine *engine){

    if(m_audioEngine){
        disconnect(m_audioEngine, &MAudioEngine::notifyEngineStatus, this, &MContentDelegate::processEngineStatus);
        disconnect(m_audioEngine, &MAudioEngine::notifyErrorStatus, this, &MContentDelegate::processErrorStatus);
        disconnect(m_audioEngine, &MAudioEngine::notifyMediaStatus, this, &MContentDelegate::processMediaStatus);
        disconnect(m_audioEngine, &MAudioEngine::notifyRequestStatus, this, &MContentDelegate::processRequestStatus);
    }

    setParent(engine);
    m_audioEngine = engine;

    connect(m_audioEngine, &MAudioEngine::notifyEngineStatus, this, &MContentDelegate::processEngineStatus);
    connect(m_audioEngine, &MAudioEngine::notifyErrorStatus, this, &MContentDelegate::processErrorStatus);
    connect(m_audioEngine, &MAudioEngine::notifyMediaStatus, this, &MContentDelegate::processMediaStatus);
    connect(m_audioEngine, &MAudioEngine::notifyRequestStatus, this, &MContentDelegate::processRequestStatus);

}
void MContentDelegate::setContent(MMediaContainer *container, MSong *song){
    m_container = container;
    m_song = song;

    if(m_audioEngine->busy()){
        setIndex(index());
    }
}
void MContentDelegate::setIndex(int idx){
    MSong *idx_song = songAt(idx);

    qDebug() << idx;

    if(idx_song){
        qDebug() << idx_song->name();
        qDebug() << "stop";
        m_song = idx_song;
        m_audioEngine->stop();
        qDebug() << "stopped";

        qDebug() << "open";
        m_audioEngine->open(m_song->path());
        qDebug() << "opened";

        qDebug() << "start";
        m_audioEngine->start();
        qDebug() << "started";


        emit notifySongChanged(m_song, m_container);
    }
}

void MContentDelegate::next(){
    setIndex(index() + 1);
}
void MContentDelegate::prev(){
    setIndex(index() - 1);
}

MAudioEngine *MContentDelegate::engine() const {
    return m_audioEngine;
}
MMediaContainer *MContentDelegate::container() const {
    return m_container;
}
MSong *MContentDelegate::song() const {
    return m_song;
}
MSong *MContentDelegate::songAt(int idx) const {
    if(m_container){
        if(idx >= 0 && idx < m_container->songs().size()){
            return m_container->songs().at(idx);
        }
    }
    return nullptr;
}
int MContentDelegate::index() const {
    return m_container ? m_container->songs().indexOf(m_song) : -1;
}

void MContentDelegate::processMediaStatus(Mpi3::MediaState state){
    Q_UNUSED(state)
}
void MContentDelegate::processEngineStatus(Mpi3::EngineState state){
    Q_UNUSED(state)

    qDebug() << "ENGINE: " << state;
//    if(state == Mpi3::EngineStopped){
//        next();
//    }
}
void MContentDelegate::processErrorStatus(Mpi3::ErrorState state){
    Q_UNUSED(state)
}
void MContentDelegate::processRequestStatus(Mpi3::EngineState state){

    qDebug() << "REQUEST: " << state;
    if(state == Mpi3::EngineActive && m_audioEngine->empty()){
        qDebug() << "REQUEST: media empty";
        setIndex(index());
    }
}
