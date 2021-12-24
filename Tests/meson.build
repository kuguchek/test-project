gtestdep = [dependency('gtest', main : true),
        dependency('gmock')
]

mock = include_directories('Mock')

exe = executable('testcase', 'Source/MobileClientTest.cpp',
        include_directories : [inc, mock], dependencies : [libdep, gtestdep, libpcre, sysrepo, libyang])

test('MobileClientTest', exe)