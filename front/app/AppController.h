#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>

class VolumeNode;
class ViewLayoutWidget;

class AppController : public QObject
{
    Q_OBJECT

public:
    explicit AppController(ViewLayoutWidget *viewLayout, QObject *parent = nullptr);

    void loadDemoVolume();

private:
    VolumeNode *m_currentVolumeNode = nullptr;
    ViewLayoutWidget *m_viewLayout = nullptr;
};

#endif // APPCONTROLLER_H
