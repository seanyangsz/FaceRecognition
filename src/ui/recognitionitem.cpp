#include "recognitionitem.h"
#include "ui_recognitionitem.h"

RecognitionItem::RecognitionItem(QWidget *parent, bool showCheckBox) :
    QWidget(parent),
    ui(new Ui::RecognitionItem)
{
    ui->setupUi(this);
    if (showCheckBox == false) {
        ui->checkBox->hide();
    }
}

RecognitionItem::~RecognitionItem()
{
    delete ui;
}

void RecognitionItem::fill(QImage image, QString name, QString time)
{
    this->image = image;
    this->name = name;
    this->time = time;

    ui->photo->setPixmap(QPixmap::fromImage(image).scaled(ui->photo->width(), ui->photo->height(),Qt::KeepAspectRatio));
    ui->name->setText(name);
    ui->time->setText(time);
}
