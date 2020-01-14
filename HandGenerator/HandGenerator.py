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
  """Uses ScriptedLoadableModule base class, available at:
  https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
  """

  def __init__(self, parent):
    ScriptedLoadableModule.__init__(self, parent)
    self.parent.title = "LeapMotionHandGenerator" # TODO make this more human readable by adding spaces
    self.parent.categories = ["Examples"]
    self.parent.dependencies = []
    self.parent.contributors = ["Leah Groves (Robarts Research Institute), Thomas Morphew (Robarts Research Institute)"] # replace with "Firstname Lastname (Organization)"
    self.parent.helpText = """
This is an example of scripted loadable module bundled in an extension.
It performs a simple thresholding on the input volume and optionally captures a screenshot.
"""
    self.parent.helpText += self.getDefaultModuleDocumentationLink()
    self.parent.acknowledgementText = """
This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
and Steve Pieper, Isomics, Inc. and was partially funded by NIH grant 3P41RR013218-12S1.
""" # replace with organization, grant and thanks.

#
# HandGeneratorWidget
#

class HandGeneratorWidget(ScriptedLoadableModuleWidget):
  """Uses ScriptedLoadableModuleWidget base class, available at:
  https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
  """
  def __init__(self, parent=None):
    ScriptedLoadableModuleWidget.__init__(self, parent)
    slicer.mymod = self
    self.connectorNode = None
    #self.trans = []
    self.spheres = None 
    self.cylinders = None
    self.generated = False 
  def setup(self):
    ScriptedLoadableModuleWidget.setup(self)
    l = slicer.modules.createmodels.logic()
    slicer.app.layoutManager().setLayout(slicer.vtkMRMLLayoutNode.SlicerLayoutOneUp3DView)

    self.parametersCollapsibleButton = ctk.ctkCollapsibleButton()
    self.parametersCollapsibleButton.text = "Parameters"
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

    self.connectButton.connect('clicked(bool)', self.onConnectButtonClicked)
    self.generateButton.connect('clicked(bool)', self.generateCylinders)
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
      self.nodes = slicer.util.getNodesByClass('vtkMRMLLinearTransformNode')
      self.n = len(self.nodes)
      l = slicer.modules.createmodels.logic()
      #mat = vtk.vtkMatrix4x4()
      self.generated = True 

      # TODO: Make sure to render the palm as well!
      for i in range (0, self.n):
        if 'Left' in self.nodes[i].GetName() or 'Right' in self.nodes[i].GetName():
          if 'Dis' in self.nodes[i].GetName() or 'Int' in self.nodes[i].GetName() or 'Prox' in self.nodes[i].GetName() or 'Meta' in self.nodes[i].GetName():
            
            # This is a temporary solution, idealy the Plus server and Leap Motion can scan the actual sizes
            # This is also subject to change for different model types that look more like a hand.
            if 'Dis' in self.nodes[i].GetName():
              self.length = 16
              self.radiusMm = 1.5
            elif 'Int' in self.nodes[i].GetName():
              self.length = 20
              self.radiusMm = 1.5
            elif 'Prox' in self.nodes[i].GetName():
              self.length = 28
              self.radiusMm = 1.5
            elif 'Meta' in self.nodes[i].GetName():
              self.length = 50
              self.radiusMm = 3

            self.cylinders = l.CreateCylinder(self.length, self.radiusMm)
            self.cylinders.SetAndObserveTransformNodeID(self.nodes[i].GetID())
            self.cylinders.SetName('Cyl'+self.nodes[i].GetName())            
            
            
            ###
            # Code below here was removed because it greatly decreased the fps/update time
            # Reason for slowdown is because of the increase in the number of transforms by a factor of 2
            # since each transform tracked by the Leap Motion sensor has one more attached to it to render 
            # a sphere which I thought would be unnecessary.
            ###

            #self.spheres = l.CreateSphere(3)
            # self.spheres = l.CreateSphere((float(self.nodes[i].GetAttribute('radiusMm'))/10)*2)
            #self.spheres.SetAndObserveTransformNodeID(self.nodes[i].GetID())
            #self.spheres.SetName('Sphere'+self.nodes[i].GetName())

            #self.Zshift = slicer.mrmlScene.AddNewNodeByClass('vtkMRMLLinearTransformNode')
            #self.Zshift.SetName('Zshift')
            #self.length = 10 
            # self.length = float(self.nodes[i].GetAttribute('lengthMm'))
            #mat.SetElement(2,3,self.length/2)
            #self.Zshift.SetAndObserveMatrixTransformToParent(mat)
            #self.Zshift.SetAndObserveTransformNodeID(self.nodes[i].GetID())
            #self.cyl = l.CreateCylinder(self.length-1.5, 1.5)
            # self.cyl = l.CreateCylinder(self.length-1.5,(float(self.nodes[i].GetAttribute('radiusMm'))/10))
            #self.cyl.SetAndObserveTransformNodeID(self.Zshift.GetID())
            #self.cyl.SetName('Cyl'+self.nodes[i].GetName())
            
            ###
            # End of code removal
            ###

      #end of for loop

      ###
      # Code below here doesn't do its intended purpose.
      # Intended purpose: Allow the user to specify which hand(s) to track in Slicer while having
      # a single XML file for the Plus Server which tracks both hands so the server never has to be restarted
      # with different XML files specifying the specific transforms. 
      #
      # Code fails because OpenIGTLink module's connector object will recreate 
      # the transform nodes that are removed when the unwanted hand re-enters the Leap Motion sensor's view
      #
      ###

      #remove nodes that are not needed for each case
      #Left hand only, remove nodes to the right
      #self.models = slicer.util.getNodesByClass('vtkMRMLModelNode')
      #for model in self.models:
      #  if case==1:
      #    if 'Right' in model.GetName():
      #      slicer.mrmlScene.RemoveNode(model)
      #  elif case==2:
      #    if 'Left' in model.GetName():
      #      slicer.mrmlScene.RemoveNode(model)
        #Case 3 is both hands so there is no removal
      #end of for loop for models

      #for transform in self.nodes:
      #  if case==1:
      #    if 'Right' in transform.GetName():
      #      slicer.mrmlScene.RemoveNode(transform)
      #  elif case==2:
      #    if 'Left' in transform.GetName():
      #      slicer.mrmlScene.RemoveNode(transform)
        #Case 3 is both hands so there is no removal
      #end of for loop for transforms

      ###
      #End of removed code
      ###
    else:
      self.nodes = slicer.util.getNodesByClass('vtkMRMLLinearTransformNode')
      self.n = len(self.nodes)
      self.models = slicer.util.getNodesByClass('vtkMRMLModelNode')
      mat = vtk.vtkMatrix4x4()
      l = slicer.modules.createmodels.logic()
      for j in range(0, len(self.models)): 
        slicer.mrmlScene.RemoveNode(self.models[j])
      for i in range (0, self.n):
        if 'Zshift' in self.nodes[i].GetName(): 
          slicer.mrmlScene.RemoveNode(self.nodes[i])
        if 'Left' in self.nodes[i].GetName() or 'Right' in self.nodes[i].GetName():
          if 'Dis' in self.nodes[i].GetName() or 'Int' in self.nodes[i].GetName() or 'Prox' in self.nodes[i].GetName() or 'Meta' in self.nodes[i].GetName():
            self.spheres = l.CreateSphere((float(self.nodes[i].GetAttribute('radiusMm'))/10)*2)
            self.spheres.SetAndObserveTransformNodeID(self.nodes[i].GetID())
            self.spheres.SetName('Sphere'+self.nodes[i].GetName())
            self.Zshift = slicer.mrmlScene.AddNewNodeByClass('vtkMRMLLinearTransformNode')
            self.Zshift.SetName('Zshift')
            self.length = float(self.nodes[i].GetAttribute('lengthMm'))
            mat.SetElement(2,3,self.length/2)
            self.Zshift.SetAndObserveMatrixTransformToParent(mat)
            self.Zshift.SetAndObserveTransformNodeID(self.nodes[i].GetID())
            self.cyl = l.CreateCylinder(self.length-1.5,(float(self.nodes[i].GetAttribute('radiusMm'))/10))
            self.cyl.SetAndObserveTransformNodeID(self.Zshift.GetID())
            self.cyl.SetName('Cyl'+self.nodes[i].GetName())
