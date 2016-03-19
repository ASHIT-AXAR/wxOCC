##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=wxOCC
ConfigurationName      :=Debug
WorkspacePath          :=/media/tuxbox/Drive/Documents/code/repos/wxOCC
ProjectPath            :=/media/tuxbox/Drive/Documents/code/repos/wxOCC/wxOCC
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=tuxbox
Date                   :=19/03/16
CodeLitePath           :=/home/tuxbox/.codelite
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
ObjectsFileList        :="wxOCC.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  $(shell wx-config --debug=yes --libs --unicode=yes)`wx-config --libs propgrid,core,base` `wx-config --cxxflags` `wx-config --libs` `wx-config --libs aui` `wx-config --libs --gl-libs` -lGL -lGLU -lglut -lstdc++ `wx-config --libs std stc` `wx-config --libs ribbon `$(shell pkg-config --libs --cflags gtk+-2.0) $(shell pkg-config --libs --cflags glib-2.0) -lTKernel -lTKMath -lTKBRep -lTKTopAlgo -lTKPrim -lTKBO -lTKOffset -lTKService -lTKV3d -lTKOpenGl -lTKFillet -lTKTObj `wx-config --libs --gl-libs` -lGL -lGLU -lglut -lstdc++
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)/usr/local/include/oce $(IncludeSwitch)/usr/include/gtk-2.0 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)X11 $(LibrarySwitch)TKService $(LibrarySwitch)TKernel $(LibrarySwitch)TKMath $(LibrarySwitch)TKBRep $(LibrarySwitch)TKV3d $(LibrarySwitch)TKOpenGl $(LibrarySwitch)TKTopAlgo $(LibrarySwitch)TKSTEP $(LibrarySwitch)TKXSBase $(LibrarySwitch)TKGeomBase $(LibrarySwitch)TKG3d $(LibrarySwitch)TKG2d $(LibrarySwitch)TKTObj 
ArLibs                 :=  "X11" "libTKService.so" "libTKernel.so" "libTKMath.so" "libTKBRep.so" "libTKV3d.so" "libTKOpenGl.so" "libTKTopAlgo.so" "libTKSTEP.so" "libTKXSBase.so" "libTKGeomBase.so" "libTKG3d.so" "libTKG2d.so" "libTKTObj.so" 
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
Objects0=$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_wxOCC.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_wxCstmTabArt.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): src/main.cpp $(IntermediateDirectory)/src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/tuxbox/Drive/Documents/code/repos/wxOCC/wxOCC/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(DependSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM "src/main.cpp"

$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix) "src/main.cpp"

$(IntermediateDirectory)/src_wxOCC.cpp$(ObjectSuffix): src/wxOCC.cpp $(IntermediateDirectory)/src_wxOCC.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/tuxbox/Drive/Documents/code/repos/wxOCC/wxOCC/src/wxOCC.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_wxOCC.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_wxOCC.cpp$(DependSuffix): src/wxOCC.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_wxOCC.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_wxOCC.cpp$(DependSuffix) -MM "src/wxOCC.cpp"

$(IntermediateDirectory)/src_wxOCC.cpp$(PreprocessSuffix): src/wxOCC.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_wxOCC.cpp$(PreprocessSuffix) "src/wxOCC.cpp"

$(IntermediateDirectory)/src_wxCstmTabArt.cpp$(ObjectSuffix): src/wxCstmTabArt.cpp $(IntermediateDirectory)/src_wxCstmTabArt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/media/tuxbox/Drive/Documents/code/repos/wxOCC/wxOCC/src/wxCstmTabArt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_wxCstmTabArt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_wxCstmTabArt.cpp$(DependSuffix): src/wxCstmTabArt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_wxCstmTabArt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_wxCstmTabArt.cpp$(DependSuffix) -MM "src/wxCstmTabArt.cpp"

$(IntermediateDirectory)/src_wxCstmTabArt.cpp$(PreprocessSuffix): src/wxCstmTabArt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_wxCstmTabArt.cpp$(PreprocessSuffix) "src/wxCstmTabArt.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


