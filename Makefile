BUILD_DIR=build/

init:
	meson $(BUILD_DIR)

build:
	ninja -C $(BUILD_DIR) all

ctags:
	ninja -C $(BUILD_DIR) ctags

clean:
	ninja -C $(BUILD_DIR) clean

compdb:
	ninja -C $(BUILD_DIR) -t compdb > compile_commands.json

doc:
	meson configure $(BUILD_DIR) -D girgen=enabled
	meson configure $(BUILD_DIR) -D gi_docgen=enabled
	meson $(BUILD_DIR) --reconfigure
	meson compile -C $(BUILD_DIR) libtribble-doc
	meson configure $(BUILD_DIR) -D girgen=disabled
	meson configure $(BUILD_DIR) -D gi_docgen=disabled
	meson $(BUILD_DIR) --reconfigure

install:
	meson configure $(BUILD_DIR) -D girgen=enabled
	meson configure $(BUILD_DIR) -D gi_docgen=enabled
	meson $(BUILD_DIR) --reconfigure
	ninja -C $(BUILD_DIR) install
	meson configure $(BUILD_DIR) -D girgen=disabled
	meson configure $(BUILD_DIR) -D gi_docgen=disabled
	meson $(BUILD_DIR) --reconfigure

test:
	meson test -C $(BUILD_DIR)

all: build ctags compdb

.PHONY: all build ctags compdb clean install doc test
