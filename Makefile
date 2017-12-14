NAME = raytracer

CC = g++

CFLAGS = -Wall -Wextra -Werror -Ofast -g -march=native -flto -mtune=native

INCLUDES_PATH = include/

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
	    Filters/Color/GreyShade.cpp \
	    Filters/Color/Negative.cpp \
	    Filters/Color/Sepia.cpp \
	    Utils/System.cpp \
	    Vec/Vec2.cpp \
	    Vec/Vec3.cpp \
	    Vec/Vec4.cpp \
	    Mat/Mat2.cpp \
	    Mat/Mat3.cpp \
	    Mat/Mat4.cpp \

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
	@echo " - Making $(NAME)"
	@$(CC) $(CFLAGS) -o $(NAME) $^ $(LIBRARY) $(FRAMEWORK)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp
	@echo " - Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $< -I$(INCLUDES_PATH)

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
	@echo " - Cleaning objs"
	@rm -f $(OBJS)

fclean: clean
	@echo " - Cleaning executable"
	@rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re odir
