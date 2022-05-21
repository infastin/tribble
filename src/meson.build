src_files = [
  'Hash.c',
  'HashTable.c',
  'HashTableIter.c',
  'Heap.c',
  'List.c',
  'Messages.c',
  'Math.c',
  'Rand.c',
  'SList.c',
  'String.c',
  'Tree.c',
  'Utils.c',
  'Vector.c',
]

header_files = [
  'Hash.h',
  'HashTable.h',
  'HashTableIter.h',
  'Heap.h',
  'List.h',
  'Macros.h',
  'Math.h',
  'Messages.h',
  'Rand.h',
  'SList.h',
  'String.h',
  'Tribble.h',
  'Tree.h',
  'Types.h',
  'Utils.h',
  'Vector.h',
]

libtribble = both_libraries('tribble', src_files, install: true)
install_headers(header_files, subdir: 'tribble')

libtribble_dep = declare_dependency(
  sources: header_files,
  link_with: libtribble,
  include_directories: include_directories('.'),
)

libtribble_gir = gnome.generate_gir(libtribble,
  sources: header_files,
  namespace: 'Tribble',
  identifier_prefix: 'Trb',
  symbol_prefix: 'trb',
  export_packages: pkg_api_name,
  nsversion: apiversion,
  install: true,
  install_dir_gir: girdir,
  extra_args: ['--quiet', '--c-include=Tribble.h'],
)
