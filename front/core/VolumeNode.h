#pragma once

#include <QObject>
#include <QString>
#include <vtkSmartPointer.h>

class vtkImageData;

class VolumeNode : public QObject
{
	Q_OBJECT

public:
	explicit VolumeNode(QObject* parent = nullptr);

	void setImageData(vtkSmartPointer<vtkImageData> imageData);
	vtkSmartPointer<vtkImageData> imageData() const;

	void setName(const QString& name);
	QString name() const;

	void setWindowLevel(double window, double level);
	double window() const;
	double level() const;

signals:
	void dataChanged();

private:
	vtkSmartPointer<vtkImageData> m_imageData;
	QString m_name;
	double m_window = 1200.0;
	double m_level = 100.0;
};