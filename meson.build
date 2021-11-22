project('mobile network', 'cpp', default_options : ['cpp_std=c++2a'])

libpcre = dependency('libpcre', required : true)
sysrepo = dependency('sysrepo-cpp', required : true)
libyang = dependency ('libyang-cpp', required : true)

subdir('MobileClient')

executable('network', 'main.cpp', include_directories : inc, dependencies : [libpcre, sysrepo, libyang], link_with : slib)