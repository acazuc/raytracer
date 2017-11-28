NAME = raytracer

CC = g++

CFLAGS = -Wall -Wextra -Werror -O2 -g -march=native -flto -mtune=native

INCLUDES_PATH = include/

SRCS_PATH = src/

SRCS_NAME = Main.cpp \
	    Raytracer.cpp \
	    Vec3.cpp \
	    Vec4.cpp \
	    Quadratic.cpp \
	    Objects/Object.cpp \
	    Objects/Sphere.cpp \
	    Objects/Cylinder.cpp \
	    Objects/Cone.cpp \
	    Objects/Plane.cpp \
	    Lights/Light.cpp \
	    Lights/PonctualLight.cpp \
	    Lights/DirectionalLight.cpp \

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_PATH = obj/

OBJS_NAME = $(SRCS_NAME:.cpp=.o)

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

LIBRARY = -lglfw

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

clean:
	@echo " - Cleaning objs"
	@rm -f $(OBJS)

fclean: clean
	@echo " - Cleaning executable"
	@rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re odir
