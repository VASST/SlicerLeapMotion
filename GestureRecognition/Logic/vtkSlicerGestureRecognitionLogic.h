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

// .NAME vtkSlicerGestureRecognitionLogic - Slicer Logic class for gesture recognition
// .SECTION Description
// This class manages the logic associated with prediciting a gesture from linear transform
// position data


#ifndef __vtkSlicerGestureRecognitionLogic_h
#define __vtkSlicerGestureRecognitionLogic_h

// Slicer includes
#include <vtkSlicerModuleLogic.h>

// VTK includes
#include <vtkCallbackCommand.h>
#include <vtkSmartPointer.h>

// STL includes
#include <memory>
#include <array>

// Local includes
#include "vtkSlicerGestureRecognitionModuleLogicExport.h"

class vtkMRMLLinearTransformNode;
class vtkMatrix4x4;
namespace GRT
{
  class ANBC;
  class DecisionTree;
  class AdaBoost;
  class KNN;
}

/// \ingroup Slicer_QtModules_LeapMotion
class VTK_SLICER_GESTURERECOGNITION_MODULE_LOGIC_EXPORT vtkSlicerGestureRecognitionLogic :
  public vtkSlicerModuleLogic
{
public:
  enum PredictionModel
  {
    GRT_ANBC,
    GRT_DECISION_TREE,
    GRT_ADABOOST,
    GRT_KNN
  };

  enum
  {
    GestureRecognizedEvent = 280023918
  };

  static vtkSlicerGestureRecognitionLogic* New();
  vtkTypeMacro(vtkSlicerGestureRecognitionLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);

  void StartPrediction(vtkMRMLLinearTransformNode* node);
  void SetPredictionModelType(PredictionModel model);

protected:
  static void OnTransformModified(vtkObject* caller, unsigned long eid, void* clientdata, void* calldata);

protected:
  vtkSlicerGestureRecognitionLogic();
  virtual ~vtkSlicerGestureRecognitionLogic();

protected:
  PredictionModel                     ModelType;
  std::unique_ptr<GRT::ANBC>          ANBCModel;
  std::unique_ptr<GRT::DecisionTree>  DecisionTreeModel;
  std::unique_ptr<GRT::AdaBoost>      AdaBoostModel;
  std::unique_ptr<GRT::KNN>           KNNModel;

  unsigned long                       ObserverTag;
  vtkMRMLLinearTransformNode*         ObservedTransformNode;
  vtkSmartPointer<vtkCallbackCommand> TransformModifiedCallback;

  vtkSlicerGestureRecognitionLogic(const vtkSlicerGestureRecognitionLogic&); // Not implemented
  void operator=(const vtkSlicerGestureRecognitionLogic&); // Not implemented
};

#endif
