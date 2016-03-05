##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=wxOCE
ConfigurationName      :=Debug
WorkspacePath          := "/home/dax/Documents/repos/wxOCE"
ProjectPath            := "/home/dax/Documents/repos/wxOCE/wxOCE"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Dax
Date                   :=05/03/16
CodeLitePath           :="/home/dax/.codelite"
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="wxOCE.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  $(shell wx-config --debug=yes --libs --unicode=yes)`wx-config --libs propgrid,core,base` `wx-config --cxxflags` `wx-config --libs` `wx-config --libs aui` `wx-config --libs --gl-libs` -lGL -lGLU -lglut -lstdc++ `wx-config --libs std stc` `wx-config --libs ribbon `$(shell pkg-config --libs --cflags gtk+-2.0) $(shell pkg-config --libs --cflags glib-2.0) -lTKernel -lTKMath -lTKBRep -lTKTopAlgo -lTKPrim -lTKBO -lTKOffset -lTKService -lTKV3d -lTKOpenGl -lTKFillet `wx-config --libs --gl-libs` -lGL -lGLU -lglut -lstdc++
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)/usr/local/include/oce $(IncludeSwitch)/usr/include/gtk-2.0 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)X11 $(LibrarySwitch)TKService $(LibrarySwitch)TKernel $(LibrarySwitch)TKMath $(LibrarySwitch)TKBRep $(LibrarySwitch)TKV3d $(LibrarySwitch)TKOpenGl $(LibrarySwitch)TKTopAlgo $(LibrarySwitch)TKSTEP $(LibrarySwitch)TKXSBase $(LibrarySwitch)TKGeomBase $(LibrarySwitch)TKG3d $(LibrarySwitch)TKG2d 
ArLibs                 :=  "X11" "libTKService.so" "libTKernel.so" "libTKMath.so" "libTKBRep.so" "libTKV3d.so" "libTKOpenGl.so" "libTKTopAlgo.so" "libTKSTEP.so" "libTKXSBase.so" "libTKGeomBase.so" "libTKG3d.so" "libTKG2d.so" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/usr/local/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(shell wx-config --cxxflags --unicode=yes --debug=yes) $(shell pkg-config --libs --cflags gtk+-2.0) $(shell pkg-config --libs --cflags glib-2.0) $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(shell wx-config --cxxflags --unicode=yes --debug=yes)  $(shell pkg-config --libs --cflags gtk+-2.0) $(shell pkg-config --libs --cflags glib-2.0) $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
LD_LIBRARY_PATH:=/home/dax/Dropbox/code/vrtx/lib/vxGUI/bin/linux/Debug:/home/dax/Dropbox/code/vrtx/lib/vxGL/bin/linux/Debug:/home/dax/Dropbox/code/vrtx/lib/vxPad/bin/linux/Debug:$LD_LIBRARY_PATH
Objects0=$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_vxOCE.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): src/main.cpp $(IntermediateDirectory)/src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dax/Documents/repos/wxOCE/wxOCE/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(DependSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM "src/main.cpp"

$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix) "src/main.cpp"

$(IntermediateDirectory)/src_vxOCE.cpp$(ObjectSuffix): src/vxOCE.cpp $(IntermediateDirectory)/src_vxOCE.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dax/Documents/repos/wxOCE/wxOCE/src/vxOCE.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_vxOCE.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_vxOCE.cpp$(DependSuffix): src/vxOCE.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_vxOCE.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_vxOCE.cpp$(DependSuffix) -MM "src/vxOCE.cpp"

$(IntermediateDirectory)/src_vxOCE.cpp$(PreprocessSuffix): src/vxOCE.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_vxOCE.cpp$(PreprocessSuffix) "src/vxOCE.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


