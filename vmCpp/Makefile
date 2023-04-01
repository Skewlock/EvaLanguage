CC		=	g++
CFLAGS	=	-Wall -Wextra -Werror
SDLFLAG	=	-lSDL2_image -g `sdl2-config --cflags --libs`
SRC_DIR	=	srcs
OBJ_DIR	=	obj
SRCS	=	$(wildcard $(SRC_DIR)/*.cpp)
OBJ		=	$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
BIN		=	vm.bin

all:	$(BIN)

$(BIN):	$(OBJ)
	$(CC) -o $(BIN) $(CFLAGS) main.cpp $(OBJ) $(SDLFLAG)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:	$(BIN)
	rm -rf $(OBJ_DIR)/*.o

fclean: clean
	rm -rf $(BIN)

re: fclean all

.PHONY: clean fclean re all