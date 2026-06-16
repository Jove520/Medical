#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "app/AppController.h"
#include "database/DatabaseManager.h"
#include "database/DicomRepository.h"
#include "viewer/ViewLayoutWidget.h"
#include "widgets/LogPanelWidget.h"
#include "widgets/ProjectPanelWidget.h"
#include "widgets/RibbonBarWidget.h"

#include <QDockWidget>
#include <QMenuBar>
#include <QSplitter>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupWindow();

    if (!DatabaseManager::instance().openDefaultDatabase()) {
        statusBar()->showMessage("Database error: " + DatabaseManager::instance().lastError());
    } else {
        statusBar()->showMessage("Database ready: " + DatabaseManager::instance().databasePath());
    }

    createMenuBar();
    createCentralArea();
    createRightPanel();
    createStatusBar();

    appController = new AppController(viewLayout, this);
    appController->loadDemoVolume();

    runDatabaseSmokeTest();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupWindow()
{
    resize(1600, 900);
    setWindowTitle("3D Lower Limb Alignment Measurement (Qt)");
}

void MainWindow::createMenuBar()
{
    menuBar()->clear();

    menuBar()->addMenu("File");
    menuBar()->addMenu("Edit");
    menuBar()->addMenu("View");
    menuBar()->addMenu("Image");
    menuBar()->addMenu("Measurement");
    menuBar()->addMenu("Tools");
    menuBar()->addMenu("Window");
    menuBar()->addMenu("Help");
}

void MainWindow::createCentralArea()
{
    QWidget *central = new QWidget(this);

    QVBoxLayout *rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    ribbonBar = new RibbonBarWidget(central);
    ribbonBar->bindMenuBar(menuBar());
    rootLayout->addWidget(ribbonBar);

    QSplitter *verticalSplitter = new QSplitter(Qt::Vertical, central);
    viewLayout = new ViewLayoutWidget(verticalSplitter);
    verticalSplitter->addWidget(viewLayout);
    verticalSplitter->addWidget(new LogPanelWidget(verticalSplitter));
    verticalSplitter->setStretchFactor(0, 12);
    verticalSplitter->setStretchFactor(1, 1);
    verticalSplitter->setSizes({ 760, 80 });
    verticalSplitter->setChildrenCollapsible(true);
    verticalSplitter->setCollapsible(0, true);
    verticalSplitter->setCollapsible(1, true);

    rootLayout->addWidget(verticalSplitter);

    setCentralWidget(central);
}

void MainWindow::createRightPanel()
{
    projectPanelDock = new QDockWidget("PROJECT MANAGEMENT", this);
    projectPanelDock->setAllowedAreas(Qt::RightDockWidgetArea);
    projectPanelDock->setMinimumWidth(360);
    projectPanelDock->setMaximumWidth(450);
    projectPanelDock->setWidget(new ProjectPanelWidget(projectPanelDock));
    addDockWidget(Qt::RightDockWidgetArea, projectPanelDock);

    if (ribbonBar) {
        ribbonBar->setProjectPanelVisible(projectPanelDock->isVisible());

        connect(ribbonBar, &RibbonBarWidget::projectPanelToggled, this, [this](bool checked) {
            projectPanelDock->setVisible(checked);
            if (checked) {
                projectPanelDock->raise();
            }
        });

        connect(projectPanelDock, &QDockWidget::visibilityChanged,
                ribbonBar, &RibbonBarWidget::setProjectPanelVisible);
    }
}

void MainWindow::createStatusBar()
{
    if (DatabaseManager::instance().databasePath().isEmpty()) {
        statusBar()->showMessage("Ready");
    } else {
        statusBar()->showMessage("Database ready: " + DatabaseManager::instance().databasePath());
    }
}

void MainWindow::runDatabaseSmokeTest()
{
    DicomRepository repository(DatabaseManager::instance().database());

    PatientRecord patient;
    patient.patientUid = "TEST_PATIENT_UID_000001";
    patient.patientId = "000001";
    patient.patientName = "Zhang San";
    patient.birthDate = "1975-06-15";
    patient.sex = "M";

    StudyRecord study;
    study.studyInstanceUid = "1.2.826.0.1.3680043.10.1000.1";
    study.patientUid = patient.patientUid;
    study.studyId = "STUDY_000001";
    study.studyDate = "20240510";
    study.studyTime = "092140";
    study.description = "CT Lower Limb";
    study.accessionNumber = "ACC_000001";
    study.institutionName = "Local Test Hospital";

    SeriesRecord series;
    series.seriesInstanceUid = "1.2.826.0.1.3680043.10.1000.1.1";
    series.studyInstanceUid = study.studyInstanceUid;
    series.seriesNumber = 1;
    series.seriesDescription = "Axial 0.6mm";
    series.modality = "CT";
    series.bodyPartExamined = "LOWER EXTREMITY";
    series.frameOfReferenceUid = "1.2.826.0.1.3680043.10.1000.frame.1";

    InstanceRecord instance;
    instance.sopInstanceUid = "1.2.826.0.1.3680043.10.1000.1.1.1";
    instance.seriesInstanceUid = series.seriesInstanceUid;
    instance.instanceNumber = 1;
    instance.filePath = "D:/TestDicom/IM000001.dcm";
    instance.fileSize = 1024;
    instance.sopClassUid = "1.2.840.10008.5.1.4.1.1.2";
    instance.transferSyntaxUid = "1.2.840.10008.1.2.1";
    instance.rows = 512;
    instance.columns = 512;

    bool ok =
        repository.insertPatient(patient) &&
        repository.insertStudy(study) &&
        repository.insertSeries(series) &&
        repository.insertInstance(instance);

    if (!ok) {
        statusBar()->showMessage("Database smoke test failed: " + repository.lastError());
        return;
    }

    statusBar()->showMessage("Database smoke test passed: " + DatabaseManager::instance().databasePath());
}
