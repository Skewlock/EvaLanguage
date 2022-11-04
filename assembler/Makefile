CC		=	g++
CFLAGS	=	-Wall -Wextra
SRC_DIR	=	srcs
OBJ_DIR	=	obj
INCLUDE	=	includes
SRCS	=	$(wildcard $(SRC_DIR)/*.cpp)
OBJ		=	$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
HEADERS	=	$(wildcard $(INCLUDE)/*hpp)
BIN		=	assembler.bin

all:	$(BIN)

$(BIN):	$(OBJ) main.cpp
	$(CC) -o $(BIN) $(CFLAGS) main.cpp $(OBJ) -I $(INCLUDE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:	$(BIN)
	rm -rf $(OBJ_DIR)/*.o

fclean: clean
	rm -rf $(BIN)

re: fclean all

.PHONY: clean fclean re all