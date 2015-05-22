set(CPP)

# forward CPP directly to the code
if(NOT "${CPP}" STREQUAL "")
    add_definitions(${CPP})
endif()

