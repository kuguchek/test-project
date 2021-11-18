project('mobile network', 'cpp')

libpcre = dependency('libpcre', required : true)
sysrepo = dependency('sysrepo', required : true)
libyang = dependency ('libyang', required : true)

inc = include_directories('MobileClient/hpp')

subdir('MobileClient')

executable('network', 'main.cpp', include_directories : inc, dependencies : [dep, libpcre, sysrepo, libyang])