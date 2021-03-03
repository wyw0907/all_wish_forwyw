
add_custom_target(
    CP_SCRIPT ALL
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/test_example.py ${PYTHON_OUTPUT_PATH}
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/test_master.py ${PYTHON_OUTPUT_PATH}
)
set (LIBRARY_OUTPUT_PATH ${PYTHON_OUTPUT_PATH})

pybind11_add_module(example SHARED example.cpp)
pybind11_add_module(master SHARED master.cpp)

target_link_libraries(master wish)