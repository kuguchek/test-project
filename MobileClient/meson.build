src = ['source/NetConfAgent.cpp', 'source/MobileClient.cpp']

inc = include_directories('hpp','api')

slib = static_library('slib', src, include_directories : inc)

libdep = declare_dependency(link_with : slib, include_directories : inc)