#ifndef MEDICALVIEWPORTWIDGET_H
#define MEDICALVIEWPORTWIDGET_H

#include <QColor>
#include <QWidget>

#include <vtkSmartPointer.h>

class QVTKOpenGLNativeWidget;
class vtkGenericOpenGLRenderWindow;
class vtkImageActor;
class vtkImageData;
class vtkImageMapToWindowLevelColors;
class vtkRenderer;

class MedicalViewportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MedicalViewportWidget(const QString& viewName,
        const QColor& borderColor,
        QWidget* parent = nullptr);

    void setImageData(vtkSmartPointer<vtkImageData> imageData);

private:
    enum class SliceOrientation {
        Axial,
        Coronal,
        Sagittal,
        ThreeD
    };

    void setupRenderer();
    void updateImageActor();
    void updateCamera(const int dims[3]);
    SliceOrientation orientation() const;

    QString m_viewName;
    QColor m_borderColor;

    QVTKOpenGLNativeWidget* m_vtkWidget = nullptr;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkImageData> m_imageData;
    vtkSmartPointer<vtkImageMapToWindowLevelColors> m_windowLevel;
    vtkSmartPointer<vtkImageActor> m_imageActor;
};

#endif // MEDICALVIEWPORTWIDGET_H
