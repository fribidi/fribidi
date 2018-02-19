with_glib = get_option('glib')
if with_glib
  glib_dep = dependency('glib-2.0', version: '>= 2.4', required: false)
  if glib_dep.found()
    tests = [
      ['BidiTest', 'test.c'],
      ['BidiCharacterTest', 'test-character.c'],
    ]
    foreach t : tests
      exe = executable(t[0],
        t[1], fribidi_unicode_version_h,
        c_args: ['-DHAVE_CONFIG_H'],
        include_directories: incs,
        dependencies: glib_dep,
        link_with: libfribidi)
      test(t[0], exe, args: files('@0@.txt'.format(t[0])))
    endforeach
  endif
endif
