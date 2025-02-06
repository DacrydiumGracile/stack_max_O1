# Обычный стек на односвязном списке нужен лишь для замеров и
# фиксации значения "обычный стек" относительно которого будут осуществляться замеры.
# он не решает задачу об нахождение максимума
add_library(default_stack_list OBJECT default_stack_list.cpp)
target_include_directories(default_stack_list PUBLIC .)
