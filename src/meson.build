src_files = [
  'Array.c',
  'Hash.c',
  'HashTable.c',
  'List.c',
  'Messages.c',
  'Polynom.c',
  'SList.c',
  'Tree.c',
  'Utils.c',
]

header_files = [
  'Array.h',
  'Hash.h',
  'HashTable.h',
  'List.h',
  'Macros.h',
  'Messages.h',
  'Polynom.h',
  'SList.h',
  'Tribble.h',
  'Tree.h',
  'Types.h',
  'Utils.h',
]

libtribble = both_libraries('tribble', src_files, install: true)
install_headers(header_files, subdir: 'tribble')

libtribble_dep = declare_dependency(
  sources: header_files,
  link_with: libtribble,
  include_directories: include_directories('.'),
)
