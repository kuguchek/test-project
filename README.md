Meson tasks:
1. Compile content of abstract_factory into "example_task" executable. Try to run it.
2. Compile content of abstract_factory/source into a static or shared library and abstract_factory/main.cpp into executable. Link library to executable. Try to run executable.
3. Compile content of abstract_factory/test into test executable. Try to run unit test.
4*. Compile content of proto_example into an executable. Try to generate task_info.pb.cc and task_info.pb.h in two ways(using custom_target() and using generator()). Try to run executable.
