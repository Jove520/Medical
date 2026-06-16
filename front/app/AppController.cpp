#include "AppController.h"

#include "core/VolumeNode.h"
#include "viewer/ViewLayoutWidget.h"
#include "viewer/VtkImageDataFactory.h"

AppController::AppController(ViewLayoutWidget *viewLayout, QObject *parent)
    : QObject(parent)
    , m_viewLayout(viewLayout)
{
}

void AppController::loadDemoVolume()
{
    if (!m_viewLayout) {
        return;
    }

    if (m_currentVolumeNode) {
        m_currentVolumeNode->deleteLater();
    }

    m_currentVolumeNode = new VolumeNode(this);
    m_currentVolumeNode->setName("Demo CT Volume");
    m_currentVolumeNode->setWindowLevel(1200.0, 100.0);
    m_currentVolumeNode->setImageData(VtkImageDataFactory::createDemoVolume());

    m_viewLayout->setVolumeNode(m_currentVolumeNode);
}
