CC := gcc
CFLAGS := -Wall -O2 -I.

BUILD_DIR := build
TARGET := excue

SRC_DIRS := client104 server104 virtual_tcp pact104 mailbox

# 根目录的源文件（除子目录外的 .c）
ROOT_SRCS := main.c

# 根目录对应的目标文件
ROOT_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(ROOT_SRCS))

.PHONY: all clean prepare build_dirs

all: prepare build_dirs $(TARGET)

# 调用子目录 make，传递 BUILD_DIR 变量
build_dirs:
	@for dir in $(SRC_DIRS); do \
		$(MAKE) -C $$dir BUILD_DIR=$(BUILD_DIR); \
	done

# 链接所有目标文件（根目录 + 子目录）
# 明确收集所有子目录的.o文件
SUBDIR_OBJS := $(foreach dir,$(SRC_DIRS),$(wildcard $(BUILD_DIR)/$(dir)/*.o))

$(TARGET): $(ROOT_OBJS) $(SUBDIR_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# 编译根目录的 .c 文件
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

prepare:
	@mkdir -p $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR) $(TARGET)
	@for dir in $(SRC_DIRS); do $(MAKE) -C $$dir clean; done



	