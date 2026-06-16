#include "VtkImageDataFactory.h"

#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>

#include <algorithm>
#include <cmath>

vtkSmartPointer<vtkImageData> VtkImageDataFactory::createDemoVolume()
{
    constexpr int width = 160;
    constexpr int height = 160;
    constexpr int depth = 120;

    auto image = vtkSmartPointer<vtkImageData>::New();
    image->SetDimensions(width, height, depth);
    image->SetSpacing(0.8, 0.8, 1.0);
    image->SetOrigin(0.0, 0.0, 0.0);
    image->AllocateScalars(VTK_SHORT, 1);

    const double cx = width * 0.5;
    const double cy = height * 0.5;
    const double cz = depth * 0.5;

    for (int z = 0; z < depth; ++z) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                auto* pixel = static_cast<short*>(image->GetScalarPointer(x, y, z));

                const double dx = (x - cx) / 52.0;
                const double dy = (y - cy) / 42.0;
                const double dz = (z - cz) / 48.0;
                const double body = dx * dx + dy * dy + dz * dz;

                short value = -1000;

                if (body < 1.0) {
                    value = 60;
                }

                const double bone1 = std::pow((x - 62.0) / 13.0, 2.0)
                    + std::pow((y - 80.0) / 13.0, 2.0)
                    + std::pow((z - cz) / 55.0, 2.0);

                const double bone2 = std::pow((x - 98.0) / 13.0, 2.0)
                    + std::pow((y - 80.0) / 13.0, 2.0)
                    + std::pow((z - cz) / 55.0, 2.0);

                if (bone1 < 1.0 || bone2 < 1.0) {
                    value = 900;
                }

                pixel[0] = value;
            }
        }
    }

    image->Modified();
    return image;
}