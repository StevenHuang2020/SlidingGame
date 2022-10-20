#pragma once
#include <Qdialog>
#include <QCamera>

#include "ui_camera_dlg.h"

QT_BEGIN_NAMESPACE
namespace Ui { class camera_dlg; }
QT_END_NAMESPACE

class Camera_dlg : public QDialog
{
public:
	Camera_dlg(QWidget* parent = Q_NULLPTR);
	~Camera_dlg() {};

public:

private slots:
	void setCamera(const QCameraInfo& cameraInfo);

	void startCamera();
	void stopCamera();

private:
	Ui::camera_dlg ui;
	QScopedPointer<QCamera> m_camera;
};

