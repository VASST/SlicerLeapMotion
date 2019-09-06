/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// GestureRecognition Logic includes
#include <vtkSlicerGestureRecognitionLogic.h>

// GestureRecognition includes
#include "qSlicerGestureRecognitionModule.h"
#include "qSlicerGestureRecognitionModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerGestureRecognitionModulePrivate
{
public:
  qSlicerGestureRecognitionModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerGestureRecognitionModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerGestureRecognitionModulePrivate::qSlicerGestureRecognitionModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerGestureRecognitionModule methods

//-----------------------------------------------------------------------------
qSlicerGestureRecognitionModule::qSlicerGestureRecognitionModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerGestureRecognitionModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerGestureRecognitionModule::~qSlicerGestureRecognitionModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerGestureRecognitionModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerGestureRecognitionModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerGestureRecognitionModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("Daniel Allen (Western University)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerGestureRecognitionModule::icon() const
{
  return QIcon(":/Icons/GestureRecognition.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerGestureRecognitionModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerGestureRecognitionModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerGestureRecognitionModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerGestureRecognitionModule
::createWidgetRepresentation()
{
  return new qSlicerGestureRecognitionModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerGestureRecognitionModule::createLogic()
{
  return vtkSlicerGestureRecognitionLogic::New();
}
