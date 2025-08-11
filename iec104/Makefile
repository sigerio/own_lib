# 顶层 Makefile

CC      := gcc
CFLAGS  := -Wall -O2 -Iinclude
BUILD_DIR := build
TARGET  := excue

# 从子目录收集源文件
SRC_DIRS := client104 server104
SRCS := $(wildcard *.c)

# 包含子目录 Makefile，它们会往 SRCS 里追加内容
include $(addsuffix /Makefile,$(SRC_DIRS))

# 把所有源文件映射到 build 目录的目标文件路径
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all clean prepare

# 在执行 all 之前先执行 prepare（创建 build 目录）
all: prepare $(TARGET)

# 链接可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# 编译规则（集中到 build 目录）
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 创建 build 目录
prepare:
	@mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
