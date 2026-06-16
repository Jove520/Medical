#include "MedicalViewportWidget.h"

#include <QVBoxLayout>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageMapper3D.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

MedicalViewportWidget::MedicalViewportWidget(const QString& viewName,
    const QColor& borderColor,
    QWidget* parent)
    : QWidget(parent)
    , m_viewName(viewName)
    , m_borderColor(borderColor)
{
    setMinimumSize(0, 0);
    setupRenderer();
}

void MedicalViewportWidget::setupRenderer()
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_vtkWidget = new QVTKOpenGLNativeWidget(this);
    layout->addWidget(m_vtkWidget);

    m_renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    m_renderer = vtkSmartPointer<vtkRenderer>::New();

    m_vtkWidget->setRenderWindow(m_renderWindow);
    m_renderWindow->AddRenderer(m_renderer);

    m_renderer->SetBackground(0.0, 0.0, 0.0);
}

void MedicalViewportWidget::setImageData(vtkSmartPointer<vtkImageData> imageData)
{
    if (!imageData.GetPointer()) {
        return;
    }

    m_imageData = imageData;
    updateImageActor();
}

MedicalViewportWidget::SliceOrientation MedicalViewportWidget::orientation() const
{
    if (m_viewName == "Axial") {
        return SliceOrientation::Axial;
    }

    if (m_viewName == "Coronal") {
        return SliceOrientation::Coronal;
    }

    if (m_viewName == "Sagittal") {
        return SliceOrientation::Sagittal;
    }

    return SliceOrientation::ThreeD;
}

void MedicalViewportWidget::updateImageActor()
{
    if (!m_imageData) {
        return;
    }

    int dims[3] = {};
    m_imageData->GetDimensions(dims);

    if (dims[0] <= 0 || dims[1] <= 0 || dims[2] <= 0) {
        return;
    }

    m_renderer->RemoveAllViewProps();

    m_windowLevel = vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();
    m_windowLevel->SetInputData(m_imageData);
    m_windowLevel->SetWindow(1200.0);
    m_windowLevel->SetLevel(100.0);
    m_windowLevel->Update();

    m_imageActor = vtkSmartPointer<vtkImageActor>::New();
    m_imageActor->GetMapper()->SetInputConnection(m_windowLevel->GetOutputPort());

    const int cx = dims[0] / 2;
    const int cy = dims[1] / 2;
    const int cz = dims[2] / 2;

    switch (orientation()) {
    case SliceOrientation::Axial:
        m_imageActor->SetDisplayExtent(0, dims[0] - 1, 0, dims[1] - 1, cz, cz);
        break;
    case SliceOrientation::Coronal:
        m_imageActor->SetDisplayExtent(0, dims[0] - 1, cy, cy, 0, dims[2] - 1);
        break;
    case SliceOrientation::Sagittal:
        m_imageActor->SetDisplayExtent(cx, cx, 0, dims[1] - 1, 0, dims[2] - 1);
        break;
    case SliceOrientation::ThreeD:
        m_imageActor->SetDisplayExtent(0, dims[0] - 1, 0, dims[1] - 1, cz, cz);
        break;
    }

    m_renderer->AddActor(m_imageActor);
    m_renderer->ResetCamera();

    updateCamera(dims);

    m_renderer->ResetCameraClippingRange();
    m_renderWindow->Render();
}

void MedicalViewportWidget::updateCamera(const int dims[3])
{
    const int cx = dims[0] / 2;
    const int cy = dims[1] / 2;
    const int cz = dims[2] / 2;

    auto* camera = m_renderer->GetActiveCamera();
    camera->ParallelProjectionOn();

    if (orientation() == SliceOrientation::Axial) {
        camera->SetPosition(cx, cy, 500);
        camera->SetFocalPoint(cx, cy, cz);
        camera->SetViewUp(0, 1, 0);
    }
    else if (orientation() == SliceOrientation::Coronal) {
        camera->SetPosition(cx, -500, cz);
        camera->SetFocalPoint(cx, cy, cz);
        camera->SetViewUp(0, 0, 1);
    }
    else {
        camera->SetPosition(500, cy, cz);
        camera->SetFocalPoint(cx, cy, cz);
        camera->SetViewUp(0, 0, 1);
    }
}
