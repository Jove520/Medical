#ifndef VTKIMAGEDATAFACTORY_H
#define VTKIMAGEDATAFACTORY_H

#include <vtkSmartPointer.h>

class vtkImageData;

class VtkImageDataFactory
{
public:
    static vtkSmartPointer<vtkImageData> createDemoVolume();
};

#endif // VTKIMAGEDATAFACTORY_H