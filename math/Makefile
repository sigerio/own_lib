.PRECIOUS: %.i %.s  # 告诉 make 不要删除 .i 和 .s 文件
OUTPATH = ./build
SAVEMIDDLE = -save-temps
SRC = $(wildcard *.c)


#预编译文件及路径--预编译
EI = $(patsubst %.c, $(OUTPATH)/%.i, $(SRC))
#汇编文件及路径--编译
SS = $(patsubst %.c, $(OUTPATH)/%.s, $(SRC))
#二进制文件--汇编
OBJ = $(patsubst %.c, $(OUTPATH)/%.o, $(SRC))
#可执行文件--链接
TARGET = $(OUTPATH)/excue



$(TARGET):$(OBJ)
	gcc $(OBJ) -o $(TARGET)

# 模式匹配规则：当make需要目标 xyz.o 时，自动生成一条 xyz.o: xyz.c 规则:
$(OUTPATH)/%.o:%.c
	gcc -c $< -o $@



.PHONY: clean
.PHONY: build
.PHONY: run
.PHONY: rebuild


build:
	@mkdir -p $(OUTPATH)
	@make

clean:
	rm -rf $(OUTPATH)

run :
	@if [ ! -d $(OUTPATH) ]; then \
	mkdir -p $(OUTPATH) ; \
	fi
	@make;
	./$(TARGET)

rebuild:
	@make clean
	@make run