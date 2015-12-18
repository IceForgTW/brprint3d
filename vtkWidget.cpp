    /*
 * This class has a deprecated library!
 * QVTKWidget is disused, so this class will not be used for long.
 * Only I'm working with her because I'm learning VTK and many Qt + VTK tutorials still use this library,
 * with time I will be carrying to the new class.
 * Thank you for your understanding.
 * Lays Rodrigues November 7,2015
 * */

/*=====================================================================

 BRPrint3D, Open Source 3D Printing Host

 Copyright (C) 2015 BRPrint3D Authors

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

#include "vtkWidget.h"

#include <vtkPolyData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkTransform.h>
#include <vtkTriangleFilter.h>
#include <vtkProperty.h>
#include <vtkAxesActor.h>
#include <vtkQuad.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSTLReader.h>

#define DEFAULTX 200
#define DEFAULTY 200
#define DEFAULTZ 200

vtkWidget::vtkWidget(QWidget *parent) : QVTKWidget(parent),
    renderer(vtkSmartPointer<vtkRenderer>::New()),
    mapperStl(vtkSmartPointer<vtkPolyDataMapper>::New()),
    mapperGcode(vtkSmartPointer<vtkPolyDataMapper>::New()),
    mapperCube(vtkSmartPointer<vtkPolyDataMapper>::New()),
    mapperFloor(vtkSmartPointer<vtkPolyDataMapper>::New()),
    actorStl(vtkSmartPointer<vtkActor>::New()),
    actorGcode(vtkSmartPointer<vtkActor>::New()),
    actorCube(vtkSmartPointer<vtkActor>::New()),
    actorFloor(vtkSmartPointer<vtkActor>::New()),
    cube(vtkSmartPointer<vtkCubeSource>::New()),
    areaX(DEFAULTX),
    areaY(DEFAULTY),
    areaZ(DEFAULTZ)
{
    GetRenderWindow()->AddRenderer(renderer);
    renderer->SetBackground(0.576,0.749,0.874);
    //actor->GetProperty()->SetColor();

    drawCube();
}

vtkWidget::~vtkWidget()
{
    
}

void vtkWidget::renderSTL(const QString& pathStl)
{
    cleanup();

    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(pathStl.toStdString().c_str());
    reader->Update();

    auto triangulate = vtkSmartPointer<vtkTriangleFilter>::New();
    triangulate->SetInputConnection(reader->GetOutputPort());
    triangulate->Update();

    vtkSmartPointer<vtkTransform> scaleSTL = vtkSmartPointer<vtkTransform>::New();
    scaleSTL->Translate(areaX/2,areaY/2,areaZ/4);
    scaleSTL->Scale(10,10,10);

    mapperStl->SetInputConnection(reader->GetOutputPort());
    actorStl->SetMapper(mapperStl);
    actorStl->SetUserTransform(scaleSTL);
    renderer->AddActor(actorStl);
    renderer->ResetCamera();
    GetRenderWindow()->Render();
}

void vtkWidget::renderGcode(const QString& text)
{
    int nrLayers = 0;
    auto points = vtkSmartPointer<vtkPoints>::New();
    double x = 0, y = 0, z = 0;
    int count = 0;
    QStringList list = text.split('\n', QString::SkipEmptyParts);

    cleanup();
    for(const auto string : list) {
        if (string.startsWith(';'))
            continue;

        QStringList aux = string.split(' ');
        for (int j = 1, end = aux.size(); j != end; j++) {
            if (aux.at(j).startsWith('X') && aux.at(j+1).startsWith('Y')) {
                x = aux.at(j).section('X',1).toDouble();
                y = aux.at(j+1).section('Y',1).toDouble();
                points->InsertPoint(count,x,y,z);
                count++;
            } else if(aux[j].startsWith('Z')) {
                z = aux.at(j).section('Z',1).toDouble();
                nrLayers++;
                points->InsertPoint(count,x,y,z);
                count++;
            }
        }
    }

    emit layersCount(nrLayers);
    auto polyLine = vtkSmartPointer<vtkPolyLine>::New();
    polyLine->GetPointIds()->SetNumberOfIds(count);
    for(unsigned int i = 0; i < count; i++)
        polyLine->GetPointIds()->SetId(i,i);

    auto cells = vtkSmartPointer<vtkCellArray>::New();
    auto polyData = vtkSmartPointer<vtkPolyData>::New();

    cells->InsertNextCell(polyLine);
    polyData->SetPoints(points);
    polyData->SetLines(cells);

    // Setup actor and mapper
    mapperGcode->SetInputData(polyData);
    actorGcode->SetMapper(mapperGcode);
    actorGcode->GetProperty()->SetLineWidth(2);
    actorGcode->GetProperty()->SetColor(0,0.5,1); // TODO: Do not use colors as constants. 
    renderer->AddActor(actorGcode);
    renderer->ResetCamera();
    GetRenderWindow()->Render();
}


void vtkWidget::cleanup()
{
    if(actorStl !=0 ) {
        renderer->RemoveActor(actorStl);
    }

    if(actorGcode !=0 ) {
        renderer->RemoveActor(actorGcode);
    }
}

void vtkWidget::drawCube()
{
    if(actorCube!=0)
        renderer->RemoveActor(actorCube);

    auto triangulate = vtkSmartPointer<vtkTriangleFilter>::New();
    auto transform = vtkSmartPointer<vtkTransform>::New();
    auto axes = vtkSmartPointer<vtkAxesActor>::New();

    cube->SetXLength(areaX);
    cube->SetYLength(areaY);
    cube->SetZLength(areaZ);
    cube->SetCenter(areaX/2, areaY/2, areaZ/2);
    cube->Update();

    triangulate->SetInputConnection(cube->GetOutputPort());
    triangulate->Update();

    mapperCube->SetInputConnection(cube->GetOutputPort());
    actorCube->SetMapper(mapperCube);
    actorCube->GetProperty()->SetRepresentationToWireframe();

    transform->Translate(-10,-10,0); // TODO: Hardcoded values
    transform->Scale(50,50,50); // TODO: Hardcoded values

    axes->SetUserTransform(transform);

    renderer->AddActor(axes);
    renderer->AddActor(actorCube);
    renderer->ResetCamera();
    GetRenderWindow()->Render();
    drawFloor();
}

void vtkWidget::updateCube(const QString& v, QChar axis)
{
    switch(axis.toLatin1()) {
        case 'X' : areaX = v.toDouble(); break;
        case 'Y' : areaY = v.toDouble(); break;
        case 'Z' : areaZ = v.toDouble(); break;
    }
    drawCube();
}

void vtkWidget::drawFloor()
{
    double p0[3] = {0.0, 0.0, 0.0};
    double p1[3] = {1.0, 0.0, 0.0};
    double p2[3] = {1.0, 1.0, 0.0};
    double p3[3] = {0.0, 1.0, 0.0};

    auto points = vtkSmartPointer<vtkPoints>::New();
    auto quad = vtkSmartPointer<vtkQuad>::New();
    auto quads = vtkSmartPointer<vtkCellArray>::New();
    auto transform = vtkSmartPointer<vtkTransform>::New();
    auto polydata = vtkSmartPointer<vtkPolyData>::New();

    // Add the points to a vtkPoints object
    points->InsertNextPoint(p0);
    points->InsertNextPoint(p1);
    points->InsertNextPoint(p2);
    points->InsertNextPoint(p3);

    // Create a quad on the four points
    quad->GetPointIds()->SetId(0,0);
    quad->GetPointIds()->SetId(1,1);
    quad->GetPointIds()->SetId(2,2);
    quad->GetPointIds()->SetId(3,3);

    // Create a cell array to store the quad in
    quads->InsertNextCell(quad);

    // Add the points and quads to the dataset
    polydata->SetPoints(points);
    polydata->SetPolys(quads);

    mapperFloor->SetInputData(polydata);

    actorFloor->SetMapper(mapperFloor);
    actorFloor->GetProperty()->SetColor(0.874,0.898,0.917);

    transform->Scale(areaX,areaY,areaZ);
    actorFloor->SetUserTransform(transform);
    renderer->AddActor(actorFloor);
    GetRenderWindow()->Render();
}
