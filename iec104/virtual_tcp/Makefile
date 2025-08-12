CC := gcc
CFLAGS := -Wall -O2 -I../include

# 允许顶层传递 SUBDIR 变量，默认值 fallback

SUBDIR := ../build


# 只匹配本目录的所有 .c 文件（不带目录前缀）
SRCS := $(wildcard *.c)

# 目标 .o 文件路径，放在顶层 build/子目录 下，保持目录结构
OBJS := $(patsubst %.c,$(SUBDIR)/$(notdir $(CURDIR))/%.o,$(SRCS))

.PHONY: all clean

all: $(OBJS)

# 编译规则
$(SUBDIR)/$(notdir $(CURDIR))/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)
