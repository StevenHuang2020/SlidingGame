#include "camera_dlg.h"
#include <QCameraInfo>

Camera_dlg::Camera_dlg(QWidget* parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setLayout(ui.gridLayout);
	setCamera(QCameraInfo::defaultCamera());
}

void Camera_dlg::setCamera(const QCameraInfo& cameraInfo)
{
	m_camera.reset(new QCamera(cameraInfo));

	m_camera->setViewfinder(ui.viewfinder);

	startCamera();
}

void Camera_dlg::startCamera()
{
	m_camera->start();
}

void Camera_dlg::stopCamera()
{
	m_camera->stop();
}