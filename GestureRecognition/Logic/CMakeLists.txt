project(vtkSlicer${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_SLICER_${MODULE_NAME_UPPER}_MODULE_LOGIC_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  )

set(${KIT}_SRCS
  vtkSlicer${MODULE_NAME}Logic.cxx
  vtkSlicer${MODULE_NAME}Logic.h
  )

set(${KIT}_TARGET_LIBRARIES
  ${ITK_LIBRARIES}
  grt
  )

#-----------------------------------------------------------------------------
SlicerMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/Models/ANBCModel.grt
  ${SlicerLeapMotion_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME}/ANBCModel.grt
  COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/Models/DecisionTreeModel.grt
  ${SlicerLeapMotion_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME}/DecisionTreeModel.grt
  COPYONLY)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/Models/ANBCModel.grt
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/Models/DecisionTreeModel.grt
  DESTINATION ${Slicer_INSTALL_QTLOADABLEMODULES_SHARE_DIR}/${MODULE_NAME} COMPONENT Runtime)
