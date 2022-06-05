src_files = [
  'trb-checked.c',
  'trb-deque.c',
  'trb-hash.c',
  'trb-hash-table.c',
  'trb-hash-table-iter.c',
  'trb-heap.c',
  'trb-list.c',
  'trb-messages.c',
  'trb-math.c',
  'trb-rand.c',
  'trb-slice.c',
  'trb-slist.c',
  'trb-string.c',
  'trb-tree.c',
  'trb-utils.c',
  'trb-vector.c',
]

header_files = [
  'trb-checked.h',
  'trb-deque.h',
  'trb-hash.h',
  'trb-hash-table.h',
  'trb-hash-table-iter.h',
  'trb-heap.h',
  'trb-list.h',
  'trb-macros.h',
  'trb-math.h',
  'trb-messages.h',
  'trb-rand.h',
  'trb-slice.h',
  'trb-slist.h',
  'trb-string.h',
  'trb-tree.h',
  'trb-types.h',
  'trb-utils.h',
  'trb-vector.h',
  'tribble.h',
]

libtribble = both_libraries('tribble', src_files, install: true)
install_headers(header_files, subdir: 'tribble')

libtribble_dep = declare_dependency(
  sources: header_files,
  link_with: libtribble,
  include_directories: include_directories('.'),
)

girgen_feature = get_option('girgen')
girgen = girgen_feature.enabled() or girgen_feature.auto()

if girgen
  libtribble_gir = gnome.generate_gir(libtribble,
    sources: header_files,
    namespace: 'Tribble',
    identifier_prefix: 'Trb',
    symbol_prefix: 'trb',
    extra_args: [
      '--quiet',
      '--c-include=tribble.h',
      '--accept-unprefixed',
    ],
    export_packages: pkg_api_name,
    nsversion: apiversion,
    install: true,
    install_dir_gir: girdir,
  )
endif
