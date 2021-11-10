gtestdep = dependency('gtest', main : true)

e = executable('testcase', 'ConcreteProductA1Test.cpp',
        include_directories : inc, dependencies : [gtestdep, dep])

test('ConcreteProductTest', e)
