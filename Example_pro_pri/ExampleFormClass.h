#ifndef EXAMPLEFORMCLASS_H
#define EXAMPLEFORMCLASS_H

#include <QWidget>

namespace Ui {
class ExampleFormClass;
}

class ExampleFormClass : public QWidget
{
    Q_OBJECT

public:
    explicit ExampleFormClass(QWidget *parent = 0);
    ~ExampleFormClass();

private:
    Ui::ExampleFormClass *ui;
};

#endif // EXAMPLEFORMCLASS_H
