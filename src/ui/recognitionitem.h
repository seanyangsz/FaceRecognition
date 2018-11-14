#ifndef RECOGNITIONITEM_H
#define RECOGNITIONITEM_H

#include <QWidget>

namespace Ui {
class RecognitionItem;
}

class RecognitionItem : public QWidget
{
    Q_OBJECT

public:
    explicit RecognitionItem(QWidget *parent = nullptr, bool showCheckBox = false);
    ~RecognitionItem();
    void fill(QImage image, QString name, QString time);
    QImage image;
    QString name;
    QString time;

private:
    Ui::RecognitionItem *ui;
};

#endif // RECOGNITIONITEM_H
