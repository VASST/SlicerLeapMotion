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
#include "vtkSlicerGestureRecognitionLogic.h"

// GRT includes
#include <GRT.h>

// MRML includes
#include <vtkMRMLLinearTransformNode.h>

// VTK includes
#include <vtkMatrix4x4.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkTransform.h>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerGestureRecognitionLogic);

namespace
{
  std::array<double, 3> vtkMatrixToEulerAngles(vtkMatrix4x4* R)
  {
    std::array<double, 3> a;

    float sy = sqrt(R->Element[0][0] * R->Element[0][0] + R->Element[1][0] * R->Element[1][0]);

    bool singular = sy < 1e-6;

    float x, y, z;
    if (!singular)
    {
      x = atan2(R->Element[2][1], R->Element[2][2]);
      y = atan2(-R->Element[2][0], sy);
      z = atan2(R->Element[1][0], R->Element[0][0]);
    }
    else
    {
      x = atan2(-R->Element[1][2], R->Element[1][1]);
      y = atan2(-R->Element[2][0], sy);
      z = 0;
    }

    // Convert Radian to degrees (normalized)
    a[0] = (x / PI);
    a[1] = (y / PI);
    a[2] = (z / PI);

    return a;
  }
}

//----------------------------------------------------------------------------
vtkSlicerGestureRecognitionLogic::vtkSlicerGestureRecognitionLogic()
  : ModelType(GRT_DECISION_TREE)
  , ANBCModel(std::make_unique<GRT::ANBC>())
  , DecisionTreeModel(std::make_unique<GRT::DecisionTree>())
  , AdaBoostModel(std::make_unique<GRT::AdaBoost>())
  , KNNModel(std::make_unique<GRT::KNN>())
{
  this->TransformModifiedCallback = vtkSmartPointer<vtkCallbackCommand>::New();
  this->TransformModifiedCallback->SetCallback(OnTransformModified);
  this->TransformModifiedCallback->SetClientData((void*)this);

  // TODO : download files so that tuning can be done regularly
  if (!this->DecisionTreeModel->load(GetModuleShareDirectory() + "/DecisionTreeModel.grt"))
  {
    vtkErrorMacro("Failed to load Decision Tree model from path: " << this->GetModuleShareDirectory() + "/DecisionTreeModel.grt");
  }

  if (!this->AdaBoostModel->load(GetModuleShareDirectory() + "/AdaBoostModel.grt"))
  {
    vtkErrorMacro("Failed to load AdaBoost model from path: " << this->GetModuleShareDirectory() + "/AdaBoostModel.grt");
  }

  // Load Adaptive Naive Bayes Classifier Model
  if (!this->ANBCModel->load(GetModuleShareDirectory() + "/ANBCModel.grt"))
  {
    vtkErrorMacro("Failed to load ANBC model from path: " << this->GetModuleShareDirectory() + "/ANBCModel.grt");
  }
  if (!this->KNNModel->load(GetModuleShareDirectory() + "/kNNModel.grt"))
  {
    vtkErrorMacro("Failed to load KNN model from path: " << this->GetModuleShareDirectory() + "/KNNModel.grt");
  }
}

//----------------------------------------------------------------------------
vtkSlicerGestureRecognitionLogic::~vtkSlicerGestureRecognitionLogic()
{
  this->ANBCModel = nullptr;
  this->DecisionTreeModel = nullptr;
  this->AdaBoostModel = nullptr;
  this->KNNModel = nullptr;
}

//----------------------------------------------------------------------------
void vtkSlicerGestureRecognitionLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerGestureRecognitionLogic::StartPrediction(vtkMRMLLinearTransformNode* transformNode)
{
  if (this->ObservedTransformNode != nullptr)
  {
    this->ObservedTransformNode->RemoveObserver(this->ObserverTag);
  }
  this->ObservedTransformNode = transformNode;
  if (this->ObservedTransformNode != nullptr)
  {
    this->ObserverTag = this->ObservedTransformNode->AddObserver(vtkMRMLLinearTransformNode::TransformModifiedEvent, this->TransformModifiedCallback);
  }
}

//----------------------------------------------------------------------------
void vtkSlicerGestureRecognitionLogic::SetPredictionModelType(PredictionModel model)
{
  this->ModelType = model;
}

//---------------------------------------------------------------------------
void vtkSlicerGestureRecognitionLogic::OnTransformModified(vtkObject* caller, unsigned long eid, void* clientdata, void* calldata)
{
  vtkSlicerGestureRecognitionLogic* self = (vtkSlicerGestureRecognitionLogic*)clientdata;

  vtkMRMLLinearTransformNode* node = static_cast<vtkMRMLLinearTransformNode*>(caller);
  vtkNew<vtkMatrix4x4> matr;
  self->ObservedTransformNode->GetMatrixTransformToWorld(matr);
  std::array<double, 3> ang = vtkMatrixToEulerAngles(matr);

  GRT::VectorFloat inputVector(3);
  inputVector.at(0) = ang[0];
  inputVector.at(1) = ang[1];
  inputVector.at(2) = ang[2];

  GRT::Classifier* classifier(nullptr);
  switch (self->ModelType)
  {
    case GRT_ANBC:
      classifier = self->ANBCModel.get();
      break;
    case GRT_DECISION_TREE:
      classifier = self->DecisionTreeModel.get();
      break;
    case GRT_ADABOOST:
      classifier = self->AdaBoostModel.get();
      break;
    case GRT_KNN:
      classifier = self->KNNModel.get();
      break;
  }

  if (classifier == nullptr)
  {
    vtkErrorWithObjectMacro(self, "Unknown prediction model requested.");
    return;
  }

  if (!classifier->predict(inputVector))
  {
    vtkErrorWithObjectMacro(self, "Failed to perform prediction");
    return;
  }
  UINT predictedClass = classifier->getPredictedClassLabel();

  // If we have predicted a valid gesture fire an event
  if (predictedClass > 0)
  {
    self->InvokeEvent(vtkSlicerGestureRecognitionLogic::GestureRecognizedEvent, &predictedClass);
  }
}
