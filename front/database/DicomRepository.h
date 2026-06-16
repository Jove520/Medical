#ifndef DICOMREPOSITORY_H
#define DICOMREPOSITORY_H

#include <QSqlDatabase>
#include <QString>

struct PatientRecord {
    QString patientUid;
    QString patientId;
    QString patientName;
    QString birthDate;
    QString sex;
};

struct StudyRecord {
    QString studyInstanceUid;
    QString patientUid;
    QString studyId;
    QString studyDate;
    QString studyTime;
    QString description;
    QString accessionNumber;
    QString institutionName;
};

struct SeriesRecord {
    QString seriesInstanceUid;
    QString studyInstanceUid;
    int seriesNumber = 0;
    QString seriesDescription;
    QString modality;
    QString bodyPartExamined;
    QString frameOfReferenceUid;
};

struct InstanceRecord {
    QString sopInstanceUid;
    QString seriesInstanceUid;
    int instanceNumber = 0;
    QString filePath;
    qint64 fileSize = 0;
    QString sopClassUid;
    QString transferSyntaxUid;
    int rows = 0;
    int columns = 0;
};

class DicomRepository
{
public:
    explicit DicomRepository(const QSqlDatabase& database);

    bool insertPatient(const PatientRecord& record);
    bool insertStudy(const StudyRecord& record);
    bool insertSeries(const SeriesRecord& record);
    bool insertInstance(const InstanceRecord& record);

    QString lastError() const;

private:
    QSqlDatabase m_database;
    QString m_lastError;
};

#endif