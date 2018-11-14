#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>
#include <QPushButton>
#include <QShortcut>
#include <QLabel>
#include <QMessageBox>
#include <QUrl>
// Local
#include "ui/CameraView.h"
#include "other/Buffer.h"
#include "helper/SharedImageBuffer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;
        CameraView* cameraView;

        void connectToCamera();
        void disconnectCamera();

    public slots:
        void showAboutDialog();
        void setFullScreen(bool);
};

#endif // MAINWINDOW_H
