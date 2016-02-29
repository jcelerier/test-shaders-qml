function(debugmode_clang theTarget)
    if(NOT APPLE AND NOT WIN32) # Only for linux.
      # TODO : http://stackoverflow.com/a/30176092/1495627
      # Need to check for the libstdc++ abi.
      # target_compile_definitions(${TheTarget} PUBLIC
      #  $<$<CONFIG:Debug>:_GLIBCXX_DEBUG>
      #  $<$<CONFIG:Debug>:_GLIBCXX_DEBUG_PEDANTIC>
      #  )
    endif()
endfunction()

function(debugmode_gcc theTarget)
    target_compile_definitions(${theTarget} PUBLIC
      $<$<CONFIG:Debug>:_GLIBCXX_DEBUG>
      $<$<CONFIG:Debug>:_GLIBCXX_DEBUG_PEDANTIC>
      )
endfunction()

function(debugmode_msvc theTarget)
endfunction()

function(debugmode_build theTarget)
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        debugmode_clang(${theTarget})
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
        debugmode_msvc(${theTarget})
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        debugmode_gcc(${theTarget})
    endif()
endfunction()
