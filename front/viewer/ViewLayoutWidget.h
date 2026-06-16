#ifndef VIEWLAYOUTWIDGET_H
#define VIEWLAYOUTWIDGET_H

#include <QMetaObject>
#include <QPointer>
#include <QWidget>

class MedicalViewportWidget;
class VolumeNode;

class ViewLayoutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ViewLayoutWidget(QWidget* parent = nullptr);

    void setVolumeNode(VolumeNode* volumeNode);

private:
    void updateViewsFromVolumeNode();

    QPointer<VolumeNode> m_volumeNode;
    QMetaObject::Connection m_volumeConnection;

    MedicalViewportWidget* m_coronalView = nullptr;
    MedicalViewportWidget* m_sagittalView = nullptr;
    MedicalViewportWidget* m_axialView = nullptr;
    MedicalViewportWidget* m_threeDView = nullptr;
};

#endif // VIEWLAYOUTWIDGET_H
