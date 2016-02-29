function(sanitize_clang theTarget)
    target_compile_options(${theTarget} PUBLIC
      $<$<CONFIG:Debug>:-fsanitize=undefined>
      )
    target_link_libraries(${theTarget} PRIVATE
      $<$<CONFIG:Debug>:-lubsan> )
endfunction()

function(sanitize_gcc theTarget)
    if(NOT ISCORE_COTIRE) ## Sanitizer won't work with PCHs
      target_compile_options(${theTarget} PUBLIC
        $<$<CONFIG:Debug>:-fsanitize=undefined>
      )
      target_link_libraries(${theTarget} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=undefined> )
    endif()
endfunction()

function(sanitize_msvc theTarget)
endfunction()

function(sanitize_build theTarget)
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        sanitize_clang(${theTarget})
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
        sanitize_msvc(${theTarget})
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        sanitize_gcc(${theTarget})
    endif()
endfunction()
