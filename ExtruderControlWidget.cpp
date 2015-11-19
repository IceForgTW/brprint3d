/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2015  <copyright holder> <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "ExtruderControlWidget.h"
#include "ui_ExtruderControlWidget.h"

ExtruderControlWidget::ExtruderControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtruderControlWidget)
{
    ui->setupUi(this);
    connect(ui->bt_homeX,SIGNAL(clicked()),this,SLOT(setHomeX()));
        connect(ui->bt_homeY,SIGNAL(clicked()),this,SLOT(setHomeY()));
        connect(ui->bt_homeZ,SIGNAL(clicked()),this,SLOT(setHomeZ()));
        connect(ui->bt_homeXYZ,SIGNAL(clicked()),this,SLOT(setHomeXYZ()));
        connect(ui->bt_leftX,SIGNAL(clicked()),this,SLOT(setPosX()));
        connect(ui->bt_rightX,SIGNAL(clicked()),this,SLOT(setPosX()));
        connect(ui->bt_upY,SIGNAL(clicked()),this,SLOT(setPosY()));
        connect(ui->bt_downY,SIGNAL(clicked()),this,SLOT(setPosY()));
        connect(ui->bt_upZ,SIGNAL(clicked()),this,SLOT(setPosZ()));
        connect(ui->bt_downZ,SIGNAL(clicked()),this,SLOT(setPosZ()));
        ui->bt_homeX->setIcon(QIcon(QPixmap(":/Icons/Icons/homeAxisX.png")));
        ui->bt_homeY->setIcon(QIcon(QPixmap(":/Icons/Icons/homeaxisY.png")));
        ui->bt_homeZ->setIcon(QIcon(QPixmap(":/Icons/Icons/homeAxisZ.png")));
        ui->bt_homeXYZ->setIcon(QIcon(QPixmap(":/Icons/Icons/homeAxisXYZ.png")));
        ui->bt_rightX->setIcon(QIcon(QPixmap(":/Icons/Icons/arrowRight.png")));
        ui->bt_leftX->setIcon(QIcon(QPixmap(":/Icons/Icons/arrowLeft.png")));
        ui->bt_upY->setIcon(QIcon(QPixmap(":/Icons/Icons/arrowUp.png")));
        ui->bt_downY->setIcon(QIcon(QPixmap(":/Icons/Icons/arrowDown.png")));
        ui->bt_upZ->setIcon(QIcon(QPixmap(":/Icons/Icons/arrowUp.png")));
        ui->bt_downZ->setIcon(QIcon(QPixmap(":/Icons/Icons/arrowDown.png")));
        ui->bt_filOneSpeed->setIcon(QIcon(QPixmap(":/Icons/Icons/arrowLeft.png")));
        ui->bt_filTwoSpeed->setIcon(QIcon(QPixmap(":/Icons/Icons/arrowRight.png")));
        ui->bt_filRetract->setIcon(QIcon(QPixmap(":/Icons/Icons/arrowUp.png")));
        ui->lb_XY->setPixmap(QPixmap(":/Icons/Icons/axisXY.png"));
        ui->lb_axisZ->setPixmap(QPixmap(":/Icons/Icons/axisZ.png"));
        ui->lb_Extruder->setPixmap(QPixmap(":/Icons/Icons/filamentRetract.png"));


}

ExtruderControlWidget::~ExtruderControlWidget()
{
    delete ui;
}

void ExtruderControlWidget::init()
{


}

//This slot update on UI the position of the Extruder
void ExtruderControlWidget::updatePos(double posX, double posY, double posZ)
{   //This function update the extruder position on the screen
    char resp[51];
    sprintf(resp, "%.3lf", posX);
    QString xx = QString::fromUtf8(resp);
    sprintf(resp, "%.3lf", posY);
    QString yy = QString::fromUtf8(resp);
    sprintf(resp, "%.3lf", posZ);
    QString zz = QString::fromUtf8(resp);
    ui->tb_axisXPos->setText(xx);
    ui->tb_axisYPos->setText(yy);
    ui->tb_axisZPos->setText(zz);

}
void ExtruderControlWidget::getPrinterObject(Repetier *pObject){
    printerObject = pObject;
}

void ExtruderControlWidget::setHomeX()
{
    printerObject->homeAxis('X');
}
void ExtruderControlWidget::setHomeY()
{
    printerObject->homeAxis('Y');
}
void ExtruderControlWidget::setHomeZ()
{
    printerObject->homeAxis('Z');
}
void ExtruderControlWidget::setHomeXYZ()
{
    printerObject->homeAllAxis();

}
void ExtruderControlWidget::setPosX()
{
    double pos = printerObject->getCurrentXPos();
    if(sender()==ui->bt_leftX)
        pos+=ui->sb_displacement->value();
    if(sender()==ui->bt_rightX)
        pos-=ui->sb_displacement->value();
    printerObject->moveAxisToPos('X',pos);
}
void ExtruderControlWidget::setPosY()
{
    double pos = printerObject->getCurrentYPos();
    if(sender()==ui->bt_upY)
        pos+=ui->sb_displacement->value();
    if(sender()==ui->bt_downY)
        pos-=ui->sb_displacement->value();
    printerObject->moveAxisToPos('Y',pos);

}
void ExtruderControlWidget::setPosZ()
{
    double pos = printerObject->getCurrentZPos();
    if(sender()==ui->bt_upZ)
        pos+=ui->sb_displacement->value();
    if(sender()==ui->bt_downZ)
        pos-=ui->sb_displacement->value();
    printerObject->moveAxisToPos('Z',pos);

}

void ExtruderControlWidget::destructPrinterObject(){
    printerObject->~Repetier();
}
