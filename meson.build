project('libtribble', 'c',
  version: '1.0.0',
  default_options: [
    'buildtype=debugoptimized',
    'warning_level=3',
    'c_std=gnu2x',
    'girgen=disabled',
    'gi_docgen=disabled',
  ],
  license: 'MIT'
)

version_arr = meson.project_version().split('.')

apiversion = '@0@.0'.format(version_arr[0])
libversion = meson.project_version()

pkg_api_name = '@0@-@1@'.format(meson.project_name(), apiversion)

add_project_arguments([
  '-pipe',
  '-Wduplicated-branches',
  '-funsigned-char',
  '-Wimplicit-fallthrough',
  '-Wmisleading-indentation',
  '-Wunused',
  '-Wno-pedantic',
  '-Wno-format-zero-length',
  '-Wimplicit',
  '-Werror=format=2',
  '-Wno-format-nonliteral',
  '-Werror=init-self',
  '-Werror=missing-include-dirs',
  '-Werror=pointer-arith',
  '-Wstrict-prototypes',
  '-Werror=implicit-function-declaration',
], language: 'c')

datadir = get_option('datadir')
libdir  = get_option('libdir')
girdir  = get_option('datadir') / 'gir-1.0'

gnome = import('gnome')

subdir('src')
subdir('doc')
subdir('test')

executable('main', 'main.c',
  dependencies: libtribble_dep,
)

pkg_config = import('pkgconfig')
pkg_config.generate(
  libraries: libtribble,
  version: libversion,
  filebase: pkg_api_name,
  name: 'tribble',
  description: 'Some C library',
)
