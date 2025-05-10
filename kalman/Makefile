.PRECIOUS: %.i %.s  # 告诉 make 不要删除 .i 和 .s 文件
OUTPATH = build
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

$(OUTPATH)/%.i:%.c
	gcc -E $< -o $@

$(OUTPATH)/%.s:$(OUTPATH)/%.i
	gcc -S $< -o $@

$(OUTPATH)/%.o:$(OUTPATH)/%.s
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
	./$(TARGET)

rebuild:
	@make clean
	@make build