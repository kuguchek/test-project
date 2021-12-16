gtestdep = [dependency('gtest', main : true),
        dependency('gmock')
]

mock = include_directories('Mock')

exe = executable('testcase', 'Source/MobileClientTest.cpp',
        include_directories : [inc, mock], dependencies : [gtestdep, libpcre, sysrepo, libyang], link_with : slib)

test('MobileClientTest', exe)