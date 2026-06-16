#include "ViewLayoutWidget.h"

#include "core/VolumeNode.h"
#include "MedicalViewportWidget.h"

#include <QColor>
#include <QHBoxLayout>
#include <QSplitter>

#include <vtkImageData.h>

ViewLayoutWidget::ViewLayoutWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto* viewerSplitter = new QSplitter(Qt::Horizontal, this);
    auto* leftSplitter = new QSplitter(Qt::Vertical, viewerSplitter);
    auto* rightSplitter = new QSplitter(Qt::Vertical, viewerSplitter);

    m_coronalView = new MedicalViewportWidget("Coronal", QColor(255, 140, 0), leftSplitter);
    m_sagittalView = new MedicalViewportWidget("Sagittal", QColor(0, 255, 80), leftSplitter);
    m_axialView = new MedicalViewportWidget("Axial", QColor(255, 0, 0), rightSplitter);
    m_threeDView = new MedicalViewportWidget("3D", QColor(255, 255, 255), rightSplitter);

    leftSplitter->addWidget(m_coronalView);
    leftSplitter->addWidget(m_sagittalView);
    leftSplitter->setStretchFactor(0, 1);
    leftSplitter->setStretchFactor(1, 1);
    leftSplitter->setSizes({ 1, 1 });

    rightSplitter->addWidget(m_axialView);
    rightSplitter->addWidget(m_threeDView);
    rightSplitter->setStretchFactor(0, 1);
    rightSplitter->setStretchFactor(1, 1);
    rightSplitter->setSizes({ 1, 1 });

    viewerSplitter->addWidget(leftSplitter);
    viewerSplitter->addWidget(rightSplitter);
    viewerSplitter->setStretchFactor(0, 1);
    viewerSplitter->setStretchFactor(1, 1);
    viewerSplitter->setSizes({ 1, 1 });

    viewerSplitter->setHandleWidth(4);
    leftSplitter->setHandleWidth(4);
    rightSplitter->setHandleWidth(4);

    viewerSplitter->setChildrenCollapsible(false);
    leftSplitter->setChildrenCollapsible(false);
    rightSplitter->setChildrenCollapsible(false);

    viewerSplitter->setStyleSheet(
        "QSplitter::handle { background: #d9dde3; }"
        "QSplitter::handle:hover { background: #3da5ff; }"
    );

    layout->addWidget(viewerSplitter);
}

void ViewLayoutWidget::setVolumeNode(VolumeNode* volumeNode)
{
    if (m_volumeConnection) {
        disconnect(m_volumeConnection);
        m_volumeConnection = {};
    }

    m_volumeNode = volumeNode;

    if (!m_volumeNode) {
        return;
    }

    m_volumeConnection = connect(m_volumeNode, &VolumeNode::dataChanged,
        this, &ViewLayoutWidget::updateViewsFromVolumeNode);

    updateViewsFromVolumeNode();
}

void ViewLayoutWidget::updateViewsFromVolumeNode()
{
    if (!m_volumeNode) {
        return;
    }

    auto imageData = m_volumeNode->imageData();
    if (!imageData.GetPointer()) {
        return;
    }

    m_coronalView->setImageData(imageData);
    m_sagittalView->setImageData(imageData);
    m_axialView->setImageData(imageData);
    m_threeDView->setImageData(imageData);
}
