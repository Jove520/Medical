#include "ProjectPanelWidget.h"

#include <QComboBox>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

ProjectPanelWidget::ProjectPanelWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 6, 8, 6);

    layout->addWidget(new QLabel("IMAGES", this));

    auto *caseBox = new QComboBox(this);
    caseBox->addItem("Zhang San (000001)");
    layout->addWidget(caseBox);

    auto *importButton = new QPushButton("Import DICOM", this);
    layout->addWidget(importButton);

    auto *imageTree = new QTreeWidget(this);
    imageTree->setHeaderHidden(true);

    auto *study = new QTreeWidgetItem(QStringList() << "2024-05-10  CT Lower Limb");
    auto *ct = new QTreeWidgetItem(QStringList() << "CT");
    ct->addChild(new QTreeWidgetItem(QStringList() << "Topogram (1)"));
    ct->addChild(new QTreeWidgetItem(QStringList() << "Axial 0.6mm (512)"));
    ct->addChild(new QTreeWidgetItem(QStringList() << "Coronal 0.6mm (512)"));
    ct->addChild(new QTreeWidgetItem(QStringList() << "Sagittal 0.6mm (512)"));
    ct->addChild(new QTreeWidgetItem(QStringList() << "3D Bone (1)"));
    study->addChild(ct);
    imageTree->addTopLevelItem(study);
    imageTree->expandAll();
    layout->addWidget(imageTree, 3);

    layout->addWidget(new QLabel("OBJECTS", this));

    auto *objects = new QListWidget(this);
    objects->addItems({
        "Right Femur",
        "Left Femur",
        "Right Tibia",
        "Left Tibia",
        "Mechanical Axis",
        "Anatomical Axis",
        "Landmarks"
    });
    layout->addWidget(objects, 2);

    layout->addWidget(new QLabel("MEASUREMENTS", this));

    auto *measurements = new QListWidget(this);
    measurements->addItems({
        "1  Landmark Registration",
        "2  Femoral Head Center",
        "3  Knee Center",
        "4  Ankle Center",
        "5  Mechanical Axis",
        "6  HKA Angle",
        "7  Export Report"
    });
    layout->addWidget(measurements, 2);

    layout->addWidget(new QLabel("PROPERTIES", this));

    auto *properties = new QTableWidget(5, 2, this);
    properties->setHorizontalHeaderLabels({"Name", "Value"});
    properties->verticalHeader()->setVisible(false);
    properties->horizontalHeader()->setStretchLastSection(true);

    properties->setItem(0, 0, new QTableWidgetItem("HKA Angle"));
    properties->setItem(0, 1, new QTableWidgetItem("12.1 deg (Varus)"));
    properties->setItem(1, 0, new QTableWidgetItem("Mechanical Axis (R)"));
    properties->setItem(1, 1, new QTableWidgetItem("6.3 deg (Varus)"));
    properties->setItem(2, 0, new QTableWidgetItem("Mechanical Axis (L)"));
    properties->setItem(2, 1, new QTableWidgetItem("-5.8 deg (Valgus)"));
    properties->setItem(3, 0, new QTableWidgetItem("Leg Length (R)"));
    properties->setItem(3, 1, new QTableWidgetItem("874.6 mm"));
    properties->setItem(4, 0, new QTableWidgetItem("Leg Length (L)"));
    properties->setItem(4, 1, new QTableWidgetItem("872.9 mm"));

    layout->addWidget(properties, 2);
}
