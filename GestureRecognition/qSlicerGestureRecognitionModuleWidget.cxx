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

// Qt includes
#include <QDebug>

// SlicerQt includes
#include <qMRMLNodeComboBox.h>

// Local includes
#include "qSlicerGestureRecognitionModuleWidget.h"
#include "ui_qSlicerGestureRecognitionModuleWidget.h"

// Logic includes
#include <vtkSlicerGestureRecognitionLogic.h>

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerGestureRecognitionModuleWidgetPrivate: public Ui_qSlicerGestureRecognitionModuleWidget
{
  Q_DECLARE_PUBLIC(qSlicerGestureRecognitionModuleWidget);
protected:
  qSlicerGestureRecognitionModuleWidget* const q_ptr;
public:
  qSlicerGestureRecognitionModuleWidgetPrivate(qSlicerGestureRecognitionModuleWidget& object);
  vtkSlicerGestureRecognitionLogic* logic() const;
};

//-----------------------------------------------------------------------------
// qSlicerGestureRecognitionModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerGestureRecognitionModuleWidgetPrivate::qSlicerGestureRecognitionModuleWidgetPrivate(qSlicerGestureRecognitionModuleWidget& object)
  : q_ptr(&object)
{

}

//-----------------------------------------------------------------------------
vtkSlicerGestureRecognitionLogic* qSlicerGestureRecognitionModuleWidgetPrivate::logic() const
{
  Q_Q(const qSlicerGestureRecognitionModuleWidget);
  return vtkSlicerGestureRecognitionLogic::SafeDownCast(q->logic());
}

//-----------------------------------------------------------------------------
// qSlicerGestureRecognitionModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerGestureRecognitionModuleWidget::qSlicerGestureRecognitionModuleWidget(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerGestureRecognitionModuleWidgetPrivate(*this))
{
}

//-----------------------------------------------------------------------------
qSlicerGestureRecognitionModuleWidget::~qSlicerGestureRecognitionModuleWidget()
{
  Q_D(qSlicerGestureRecognitionModuleWidget);

  QObject::disconnect(d->pushButton, &QPushButton::clicked, this, &qSlicerGestureRecognitionModuleWidget::OnPredictButtonClicked);
}

//-----------------------------------------------------------------------------
void qSlicerGestureRecognitionModuleWidget::OnPredictButtonClicked()
{
  Q_D(qSlicerGestureRecognitionModuleWidget);

  d->logic()->StartPrediction((vtkMRMLLinearTransformNode*)d->transformDropDown->currentNode());
}

//----------------------------------------------------------------------------
void qSlicerGestureRecognitionModuleWidget::OnTransformNodeChanged(vtkMRMLNode* node)
{
  Q_D(qSlicerGestureRecognitionModuleWidget);

  vtkMRMLLinearTransformNode* tNode = vtkMRMLLinearTransformNode::SafeDownCast(node);
  if (tNode == nullptr)
  {
    qCritical() << "Invalid node in combo box! How did this happen.";
    d->pushButton->setEnabled(false);
    return;
  }
  else
  {
    d->pushButton->setEnabled(true);
  }
}

//-----------------------------------------------------------------------------
void qSlicerGestureRecognitionModuleWidget::setup()
{
  Q_D(qSlicerGestureRecognitionModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  // Wire up transform nodes to dropdown
  d->transformDropDown->setNodeTypes(QStringList() << (QString)"vtkMRMLLinearTransformNode");
  d->transformDropDown->setSelectNodeUponCreation(true);
  d->transformDropDown->setAddEnabled(false);
  d->transformDropDown->setRemoveEnabled(false);
  d->transformDropDown->setNoneEnabled(false);
  d->transformDropDown->setMRMLScene(this->mrmlScene());
  d->transformDropDown->setToolTip("Pick a transform to observe");

  QObject::connect(d->pushButton, &QPushButton::clicked, this, &qSlicerGestureRecognitionModuleWidget::OnPredictButtonClicked);
  QObject::connect(d->transformDropDown, static_cast<void (qMRMLNodeComboBox::*)(vtkMRMLNode*)>(&qMRMLNodeComboBox::currentNodeChanged), this, &qSlicerGestureRecognitionModuleWidget::OnTransformNodeChanged);
}
