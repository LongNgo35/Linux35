CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
LIB_DIR := $(CUR_DIR)/lib
STATIC_LIB_DIR := $(LIB_DIR)/static_lib
DYNAMIC_LIB_DIR := $(LIB_DIR)/dynamic_lib

build_static_objs:
	gcc -c -I$(INC_DIR) main.c                       -o $(OBJ_DIR)/main.o
	gcc -c -I$(INC_DIR) $(SRC_DIR)/hello_universe.c  -o $(OBJ_DIR)/hello_universe.o
	gcc -c -I$(INC_DIR) $(SRC_DIR)/hello_world.c     -o $(OBJ_DIR)/hello_world.o

build_dynamic_objs:
	gcc -c -I$(INC_DIR) -fPIC   main.c                       -o $(OBJ_DIR)/main.o
	gcc -c -I$(INC_DIR) -fPIC   $(SRC_DIR)/hello_universe.c  -o $(OBJ_DIR)/hello_universe.o
	gcc -c -I$(INC_DIR) -fPIC   $(SRC_DIR)/hello_world.c     -o $(OBJ_DIR)/hello_world.o

create_static_library:
	ar rcs $(STATIC_LIB_DIR)/libhello.a $(OBJ_DIR)/hello_universe.o $(OBJ_DIR)/hello_world.o

create_dynamic_library:
	gcc -shared $(OBJ_DIR)/hello_universe.o $(OBJ_DIR)/hello_world.o -o $(DYNAMIC_LIB_DIR)/libhello_dynamic.so

statically_linking:
	gcc $(OBJ_DIR)/main.o -L$(STATIC_LIB_DIR) -lhello -o main

# It's know which .so file need to be use to link, but in runtime, we need to specify dynamic library folder location to compile
# chi cqan biet link voi thu vien nao, luc chay se khong biet can lay thu vien o duong dan nao => can su dung LD_LIBRARY_PATH=.... ./main de bien dich
dynamically_linking:
	gcc $(OBJ_DIR)/main.o -L$(DYNAMIC_LIB_DIR) -lhello_dynamic -o main

normal_linking:
	gcc $(OBJ_DIR)/main.o $(OBJ_DIR)/hello_universe.o $(OBJ_DIR)/hello_world.o -o main

clean:
	rm -rf $(OBJ_DIR)/main.o $(OBJ_DIR)/hello_universe.o $(OBJ_DIR)/hello_world.o main