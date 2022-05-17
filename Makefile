BUILD_DIR=build/

init:
	meson $(BUILD_DIR)

build:
	meson compile -C $(BUILD_DIR)

ctags:
	ninja -C $(BUILD_DIR) ctags

clean:
	ninja -C $(BUILD_DIR) clean

compdb:
	ninja -C $(BUILD_DIR) -t compdb > compile_commands.json

install:
	ninja -C $(BUILD_DIR) install

all: build ctags compdb

.PHONY: all build ctags compdb clean install
