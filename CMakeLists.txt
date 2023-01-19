cmake_minimum_required(VERSION 3.10)
project(bongocat)
enable_language(C CXX)
set(CXX      g++)
set(CXXFLAGS -std=c++17 -O2)
set(LDFLAGS  -lxdo -lsfml-graphics -lsfml-window -lsfml-system -lSDL2 -lX11 -lXrandr)
set(BUILD    ./build)
set(OBJDIR   $(BUILD)/objects)
set(APPDIR   $(BUILD)/bin)
set(TARGET   bongocat)
set(INCLUDE  -Iinclude/)
set(SRC
	$(wildcard src/custom/*.cpp)
	$(wildcard src/*.cpp)
)
set(OBJECTS      $(SRC:%.cpp=$(OBJDIR)/%.o))
set(DEPENDENCIES $(OBJECTS:.o=.d))

add_executable(bongocat ${SRC})

set_target_properties(bongocat CXXFLAGS)
set_target_properties(bongocat LDFLAGS)
set_target_properties(bongocat INCLUDE)

#all: build $(APPDIR)/$(TARGET)

# rebuild only one file based off object file
#$(OBJDIR)/%.o: %.cpp
#	@mkdir -p $(@D)
#	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@


#$(APPDIR)/$(TARGET): $(OBJECTS)
#	@mkdir -p $(@D)
#	$(CXX) $(CXXFLAGS) -o $(APPDIR)/$(TARGET) $^ $(LDFLAGS)

#-include $(DEPENDENCIES)

.PHONY: all build clean debug release info

build:
	@mkdir -p $(APPDIR)
	@mkdir -p $(OBJDIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJDIR)/*
	-@rm -rvf $(APPDIR)/*

info:
	# TODO: Add color
	@echo -ne "[*] Application dir: ${APPDIR}     "
	@echo -ne "[*] Object dir:      ${OBJDIR}     "
	@echo -ne "[*] Sources:         ${SRC}         "
	@echo -ne "[*] Objects:         ${OBJECTS}     "
	@echo -ne "[*] Dependencies:    ${DEPENDENCIES}"
