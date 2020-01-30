import sys 
import os
import unittest
import vtk, qt, ctk, slicer
from slicer.ScriptedLoadableModule import *
import logging
import numpy as np 
#
# HandGenerator
#
class HandGenerator(ScriptedLoadableModule):
  def __init__(self, parent):
    ScriptedLoadableModule.__init__(self, parent)
    self.parent.title = "Hand Generator"
    self.parent.categories = ["IGT"]
    self.parent.dependencies = ["OpenIGTLinkIF"]
    self.parent.contributors = ["Leah Groves (Robarts Research Institute), Thomas Morphew (Robarts Research Institute)"]
    self.parent.helpText = """This module creates a number of models (cylinders and spheres), and parents new transforms to those models in order to mimic the human hand. These transforms are then driven by the Leap Motion device."""
    self.parent.helpText += self.getDefaultModuleDocumentationLink()
    self.parent.acknowledgementText = """Thanks to the VASST Lab for its support."""

#
# HandGeneratorWidget
#
class HandGeneratorWidget(ScriptedLoadableModuleWidget):
  def __init__(self, parent=None):
    ScriptedLoadableModuleWidget.__init__(self, parent)

    self.connectorNode = None
    self.generated = False

  def setup(self):
    ScriptedLoadableModuleWidget.setup(self)

    self.parametersCollapsibleButton = ctk.ctkCollapsibleButton()
    self.parametersCollapsibleButton.text = "Actions"
    self.layout.addWidget(self.parametersCollapsibleButton)
    
    self.parametersFormLayout = qt.QFormLayout(self.parametersCollapsibleButton)
     
    self.connectButton = qt.QPushButton()
    self.connectButton.setDefault(False)
    self.connectButton.text = "Click to connect" 
    self.parametersFormLayout.addWidget(self.connectButton)
    
    self.pathText = qt.QLabel("Please place hands within view")
    self.parametersFormLayout.addRow(self.pathText)
    
    self.layout.addStretch(1)

    self.generateButton = qt.QPushButton()
    self.generateButton.setDefault(False)
    self.generateButton.text = "Generate Hands" 
    self.parametersFormLayout.addWidget(self.generateButton)

    self.generateModelButton = qt.QPushButton()
    self.generateModelButton.setDefault(False)
    self.generateModelButton.text = "Generate Hand Models" 
    self.parametersFormLayout.addWidget(self.generateModelButton)

    self.connectButton.connect('clicked(bool)', self.onConnectButtonClicked)
    self.generateButton.connect('clicked(bool)', self.generateCylinders)
    self.generateButton.connect('clicked(bool)', self.generateModels)
    self.layout.addStretch(1)

  def onConnectButtonClicked(self):
    if self.connectorNode is not None: 
      self.connectorNode = None
      self.connectCheck = 1
      self.connectButton.text = 'Click to connect'
    else:
      self.connectorNode = slicer.vtkMRMLIGTLConnectorNode()
      slicer.mrmlScene.AddNode(self.connectorNode) 
      self.connectorNode.SetTypeClient('localhost', 18944)
      self.connectorNode.Start() 
      self.connectCheck = 0  
      self.connectButton.text = 'Connected'

  def generateCylinders(self):
    if self.generated == False:
      nodes = slicer.util.getNodesByClass('vtkMRMLLinearTransformNode')
      l = slicer.modules.createmodels.logic()

      # TODO: Make sure to render the palm as well!
      for i in range (0, len(nodes)):
        if 'Left' in nodes[i].GetName() or 'Right' in nodes[i].GetName():
          if 'Dis' in nodes[i].GetName() or 'Int' in nodes[i].GetName() or 'Prox' in nodes[i].GetName() or 'Meta' in nodes[i].GetName():
            
            # This is a temporary solution, idealy the Plus server and Leap Motion can scan the actual sizes
            # This is also subject to change for different model types that look more like a hand.
            if 'Dis' in nodes[i].GetName():
              length = 16
              radiusMm = 1.5
            elif 'Int' in nodes[i].GetName():
              length = 20
              radiusMm = 1.5
            elif 'Prox' in nodes[i].GetName():
              length = 28
              radiusMm = 1.5
            elif 'Meta' in nodes[i].GetName():
              length = 50
              radiusMm = 3

            cylinder = l.CreateCylinder(length, radiusMm)
            cylinder.SetAndObserveTransformNodeID(nodes[i].GetID())
            cylinder.SetName('LHG_Cyl_'+nodes[i].GetName())            
      self.generated = True
    else:
      nodes = slicer.util.getNodesByClass('vtkMRMLLinearTransformNode')
      models = slicer.util.getNodesByClass('vtkMRMLModelNode')
      n = 0
      mat = vtk.vtkMatrix4x4()
      l = slicer.modules.createmodels.logic()

      for j in range(0, len(models)):
        if 'LHG_' in models[j].GetName():
          slicer.mrmlScene.RemoveNode(models[j])

      for i in range (0, len(nodes)):
        if 'LHG_Zshift' in nodes[i].GetName(): 
          slicer.mrmlScene.RemoveNode(nodes[i])
      self.generated = False
      self.generateCylinders()


  def generateModels(self):
    if self.generated == False:
      self.nodes = slicer.util.getNodesByClass('vtkMRMLLinearTransformNode')
      self.n = len(self.nodes)
      l = slicer.modules.createmodels.logic()
      mat = vtk.vtkMatrix4x4()
      mat.SetElement(2,2,-1)
      mat.SetElement(0,0,-1)
      self.Xflip = slicer.mrmlScene.AddNewNodeByClass('vtkMRMLLinearTransformNode')
      self.Xflip.SetName('LHG_Xflip')
      self.Xflip.SetAndObserveMatrixTransformToParent(mat)
      self.generated = True 
      self.resourcePath = os.path.dirname(os.path.abspath(__file__))
      print(self.resourcePath)


      for i in range (0, self.n):
        if 'Left' in self.nodes[i].GetName() or 'Right' in self.nodes[i].GetName():
          if 'Dis' in self.nodes[i].GetName() or 'Int' in self.nodes[i].GetName() or 'Prox' in self.nodes[i].GetName() or 'Meta' in self.nodes[i].GetName() or 'Palm' in self.nodes[i].GetName():
            
            
            print('Resources\\' + self.nodes[i].GetName() + ".stl")
            self.tempModel = slicer.util.loadModel(os.path.join(self.resourcePath, 'Resources\\' + self.nodes[i].GetName() + ".stl"))
            self.nodes[i].SetAndObserveTransformNodeID(self.Xflip.GetID())
            self.tempModel.SetAndObserveTransformNodeID(self.nodes[i].GetID())
            self.tempModel.SetName('LHG_Seg' + self.nodes[i].GetName())
          
    else:
      self.nodes = slicer.util.getNodesByClass('vtkMRMLLinearTransformNode')
      self.n = len(self.nodes)
      self.models = slicer.util.getNodesByClass('vtkMRMLModelNode')
      if self.Xflip is None:
        mat = vtk.vtkMatrix4x4()
        mat.SetElement(2,2,-1)
        mat.SetElement(0,0,-1)
        self.Xflip = slicer.mrmlScene.AddNewNodeByClass('vtkMRMLLinearTransformNode')
        self.Xflip.SetName('LHG_Xflip')
        self.Xflip.SetAndObserveMatrixTransformToParent(mat)
      for j in range(0, len(self.models)): 
        slicer.mrmlScene.RemoveNode(self.models[j])
      for i in range (0, self.n):
        if 'Left' in self.nodes[i].GetName() or 'Right' in self.nodes[i].GetName():
          if 'Dis' in self.nodes[i].GetName() or 'Int' in self.nodes[i].GetName() or 'Prox' in self.nodes[i].GetName() or 'Meta' in self.nodes[i].GetName():
            print('Resources\\' + self.nodes[i].GetName() + ".stl")
            self.tempModel = slicer.util.loadModel(os.path.join(self.resourcePath, 'Resources\\' + self.nodes[i].GetName() + ".stl"))
            self.nodes[i].SetAndObserveTransformNodeID(self.Xflip.GetID())
            self.tempModel.SetAndObserveTransformNodeID(self.nodes[i].GetID())
            self.tempModel.SetName('LHG_Seg' + self.nodes[i].GetName())
