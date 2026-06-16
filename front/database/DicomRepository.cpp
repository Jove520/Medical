#include "DicomRepository.h"

#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>

DicomRepository::DicomRepository(const QSqlDatabase& database)
    :m_database(database)
{
}

bool DicomRepository::insertPatient(const PatientRecord& record)
{
    QSqlQuery query(m_database);
    query.prepare(R"(
        INSERT OR IGNORE INTO patients (
            patient_uid, patient_id, patient_name,
            patient_birth_date, patient_sex, created_at
        ) VALUES (?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(record.patientUid);
    query.addBindValue(record.patientId);
    query.addBindValue(record.patientName);
    query.addBindValue(record.birthDate);
    query.addBindValue(record.sex);
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool DicomRepository::insertStudy(const StudyRecord& record)
{
    QSqlQuery query(m_database);
    query.prepare(R"(
        INSERT OR IGNORE INTO studies (
            study_instance_uid, patient_uid, study_id,
            study_date, study_time, study_description,
            accession_number, institution_name, created_at
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(record.studyInstanceUid);
    query.addBindValue(record.patientUid);
    query.addBindValue(record.studyId);
    query.addBindValue(record.studyDate);
    query.addBindValue(record.studyTime);
    query.addBindValue(record.description);
    query.addBindValue(record.accessionNumber);
    query.addBindValue(record.institutionName);
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool DicomRepository::insertSeries(const SeriesRecord& record)
{
    QSqlQuery query(m_database);
    query.prepare(R"(
        INSERT OR IGNORE INTO series (
            series_instance_uid, study_instance_uid, series_number,
            series_description, modality, body_part_examined,
            frame_of_reference_uid, created_at
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(record.seriesInstanceUid);
    query.addBindValue(record.studyInstanceUid);
    query.addBindValue(record.seriesNumber);
    query.addBindValue(record.seriesDescription);
    query.addBindValue(record.modality);
    query.addBindValue(record.bodyPartExamined);
    query.addBindValue(record.frameOfReferenceUid);
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));

    if(!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool DicomRepository::insertInstance(const InstanceRecord& record)
{
    QSqlQuery query(m_database);
    query.prepare(R"(
            INSERT OR IGNORE INTO instances (
                sop_instance_uid, series_instance_uid, instance_number,
                file_path, file_size, sop_class_uid, transfer_syntax_uid,
                rows, columns, created_at
            ) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(record.sopInstanceUid);
    query.addBindValue(record.seriesInstanceUid);
    query.addBindValue(record.instanceNumber);
    query.addBindValue(record.filePath);
    query.addBindValue(record.fileSize);
    query.addBindValue(record.sopClassUid);
    query.addBindValue(record.transferSyntaxUid);
    query.addBindValue(record.rows);
    query.addBindValue(record.columns);
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));

    if(!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

QString DicomRepository::lastError() const
{
    return m_lastError;
}
