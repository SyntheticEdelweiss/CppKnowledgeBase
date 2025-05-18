#include "ExampleFormClass.h"
#include "ui_ExampleFormClass.h"

ExampleFormClass::ExampleFormClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExampleFormClass)
{
    ui->setupUi(this);
}

ExampleFormClass::~ExampleFormClass()
{
    delete ui;
}
