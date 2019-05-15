NAME = raytracer

CXX = g++

CXXFLAGS = -Wall -Wextra -Ofast -g -flto -march=native -finline

INCLUDES = -I include

SRCS_PATH = src/

SRCS_NAME = Main.cpp \
	    Raytracer.cpp \
	    Quadratic.cpp \
	    Image.cpp \
	    PNG.cpp \
	    Objects/Object.cpp \
	    Objects/Sphere.cpp \
	    Objects/Cylinder.cpp \
	    Objects/Cone.cpp \
	    Objects/Plane.cpp \
	    Objects/Triangle.cpp \
	    Lights/Light.cpp \
	    Lights/PonctualLight.cpp \
	    Lights/DirectionalLight.cpp \
	    Filters/Fxaa.cpp \
	    Filters/Sobel.cpp \
	    Filters/Cel.cpp \
	    Filters/Fisheye.cpp \
	    Filters/Blur.cpp \
	    Filters/Gamma.cpp \
	    Filters/Fog.cpp \
	    Filters/Color/GreyShade.cpp \
	    Filters/Color/Negative.cpp \
	    Filters/Color/Sepia.cpp \
	    Utils/System.cpp \

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_PATH = obj/

OBJS_NAME = $(SRCS_NAME:.cpp=.o)

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

LIBRARY = -lglfw
LIBRARY+= -lpng
LIBRARY+= -lz

FRAMEWORK = -lGL
FRAMEWORK+= -lX11
FRAMEWORK+= -lXrandr
FRAMEWORK+= -lXinerama
FRAMEWORK+= -lXcursor
FRAMEWORK+= -lXxf86vm
FRAMEWORK+= -ldl
FRAMEWORK+= -lrt
FRAMEWORK+= -lm
FRAMEWORK+= -lpthread

all: odir $(NAME)

$(NAME): $(OBJS)
	@echo "LD $(NAME)"
	@$(CXX) $(CXXFLAGS) -o $(NAME) $^ $(LIBRARY) $(FRAMEWORK)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp
	@echo "CXX $<"
	@$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCLUDES)

odir:
	@mkdir -p $(OBJS_PATH)
	@mkdir -p $(OBJS_PATH)Objects
	@mkdir -p $(OBJS_PATH)Lights
	@mkdir -p $(OBJS_PATH)Filters
	@mkdir -p $(OBJS_PATH)Filters/Color
	@mkdir -p $(OBJS_PATH)Utils
	@mkdir -p $(OBJS_PATH)Vec
	@mkdir -p $(OBJS_PATH)Mat

clean:
	@rm -f $(OBJS)
	@rm -f $(NAME)

re: clean all

.PHONY: clean re odir
