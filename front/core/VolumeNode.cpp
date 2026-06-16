#include "core/VolumeNode.h"

#include <vtkImageData.h>

VolumeNode::VolumeNode(QObject* parent)
    : QObject(parent)
{
}

void VolumeNode::setImageData(vtkSmartPointer<vtkImageData> imageData)
{
    if (m_imageData == imageData) {
        return;
    }

    m_imageData = imageData;
    emit dataChanged();
}

vtkSmartPointer<vtkImageData> VolumeNode::imageData() const
{
    return m_imageData;
}

void VolumeNode::setName(const QString& name)
{
    if (m_name == name) {
        return;
    }

    m_name = name;
    emit dataChanged();
}

QString VolumeNode::name() const
{
    return m_name;
}

void VolumeNode::setWindowLevel(double window, double level)
{
    if (m_window == window && m_level == level) {
        return;
    }

    m_window = window;
    m_level = level;
    emit dataChanged();
}

double VolumeNode::window() const
{
    return m_window;
}

double VolumeNode::level() const
{
    return m_level;
}
