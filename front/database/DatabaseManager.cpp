#include "DatabaseManager.h"

#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QStringList>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager manager;
    return manager;
}

bool DatabaseManager::openDefaultDatabase()
{
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (dataDir.isEmpty()) {
        dataDir = QDir::currentPath() + "/data";
    }

    QDir dir;
    if (!dir.mkpath(dataDir)) {
        m_lastError = "Failed to create database directory: " + dataDir;
        return false;
    }

    m_databasePath = dataDir + "/dicom_index.db";

    if (QSqlDatabase::contains("main_connection")) {
        m_database = QSqlDatabase::database("main_connection");
    } else {
        m_database = QSqlDatabase::addDatabase("QSQLITE", "main_connection");
    }

    m_database.setDatabaseName(m_databasePath);

    if (!m_database.open()) {
        m_lastError = m_database.lastError().text();
        return false;
    }

    QSqlQuery query(m_database);
    query.exec("PRAGMA foreign_keys = ON;");

    return initializeSchema();
}

bool DatabaseManager::initializeSchema()
{
    if (!m_database.isOpen()) {
        m_lastError = "Database is not open.";
        return false;
    }

    if (!m_database.transaction()) {
        m_lastError = m_database.lastError().text();
        return false;
    }

    const QStringList sqlList = {
        R"(
        CREATE TABLE IF NOT EXISTS schema_info (
            key TEXT PRIMARY KEY,
            value TEXT NOT NULL
        );
        )",

        R"(
        CREATE TABLE IF NOT EXISTS patients (
            patient_uid TEXT PRIMARY KEY,
            patient_id TEXT,
            patient_name TEXT,
            patient_birth_date TEXT,
            patient_sex TEXT,
            created_at TEXT NOT NULL
        );
        )",

        R"(
        CREATE TABLE IF NOT EXISTS studies (
            study_instance_uid TEXT PRIMARY KEY,
            patient_uid TEXT NOT NULL,
            study_id TEXT,
            study_date TEXT,
            study_time TEXT,
            study_description TEXT,
            accession_number TEXT,
            institution_name TEXT,
            created_at TEXT NOT NULL,
            FOREIGN KEY(patient_uid) REFERENCES patients(patient_uid)
        );
        )",

        R"(
        CREATE TABLE IF NOT EXISTS series (
            series_instance_uid TEXT PRIMARY KEY,
            study_instance_uid TEXT NOT NULL,
            series_number INTEGER,
            series_date TEXT,
            series_time TEXT,
            series_description TEXT,
            modality TEXT,
            body_part_examined TEXT,
            frame_of_reference_uid TEXT,
            image_count INTEGER DEFAULT 0,
            created_at TEXT NOT NULL,
            FOREIGN KEY(study_instance_uid) REFERENCES studies(study_instance_uid)
        );
        )",

        R"(
        CREATE TABLE IF NOT EXISTS instances (
            sop_instance_uid TEXT PRIMARY KEY,
            series_instance_uid TEXT NOT NULL,
            import_batch_id INTEGER,
            instance_number INTEGER,
            file_path TEXT NOT NULL,
            file_size INTEGER,
            file_modified_time TEXT,
            sop_class_uid TEXT,
            transfer_syntax_uid TEXT,
            rows INTEGER,
            columns INTEGER,
            pixel_spacing TEXT,
            slice_thickness TEXT,
            image_position_patient TEXT,
            image_orientation_patient TEXT,
            window_center TEXT,
            window_width TEXT,
            created_at TEXT NOT NULL,
            FOREIGN KEY(series_instance_uid) REFERENCES series(series_instance_uid)
        );
        )",

        R"(
        CREATE TABLE IF NOT EXISTS dicom_tags (
            sop_instance_uid TEXT NOT NULL,
            tag TEXT NOT NULL,
            keyword TEXT,
            value TEXT,
            PRIMARY KEY(sop_instance_uid, tag),
            FOREIGN KEY(sop_instance_uid) REFERENCES instances(sop_instance_uid)
        );
        )",

        R"(
        CREATE TABLE IF NOT EXISTS import_batches (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            source_directory TEXT NOT NULL,
            imported_file_count INTEGER DEFAULT 0,
            skipped_file_count INTEGER DEFAULT 0,
            failed_file_count INTEGER DEFAULT 0,
            started_at TEXT NOT NULL,
            finished_at TEXT
        );
        )"
    };

    for (const QString& sql : sqlList) {
        if (!executeSql(sql)) {
            m_database.rollback();
            return false;
        }
    }

    if (!executeSql(
            "INSERT OR IGNORE INTO schema_info(key, value) "
            "VALUES('schema_version', '1');")) {
        m_database.rollback();
        return false;
    }

    if (!m_database.commit()) {
        m_lastError = m_database.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::executeSql(const QString& sql)
{
    QSqlQuery query(m_database);

    if (!query.exec(sql)) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

QString DatabaseManager::databasePath() const
{
    return m_databasePath;
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}

QSqlDatabase DatabaseManager::database() const
{
    return m_database;
}
