/*=====================================================================

 BRPrint3D, Open Source 3D Printing Host

 (c) 2015 BRPrint3D Authors

 This file is part of the BRPrint3D project

 BRPrint3D is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 BRPrint3D is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with BRPrint3D. If not, see <http://www.gnu.org/licenses/>.

 ======================================================================*/

#include "warning.h"
#include "ui_warning.h"

warning::warning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::warning)
{
    ui->setupUi(this);
    connect(ui->bt_ok,&QPushButton::clicked,this,&warning::handleEvent);
    connect(ui->bt_off,&QPushButton::clicked,this,&warning::handleEvent);
}

warning::~warning()
{
    delete ui;
}

void warning::handleEvent(){
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(btn == ui->bt_off){
        emit switchoff();
    }
    close();
}
