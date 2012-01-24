project(libkoralle)

set(koralle_LIB_SRCS
    riffstreamreader.cpp
    riffstreamreader_p.cpp
)

add_library(${koralle_LIB_NAME}  SHARED ${koralle_LIB_SRCS})

target_link_libraries(${koralle_LIB_NAME}
    ${QT_QTCORE_LIBRARY}
)

set_target_properties(${koralle_LIB_NAME}  PROPERTIES
    VERSION    ${koralle_LIB_LIBVERSION}
    SOVERSION  ${koralle_LIB_SOVERSION}
)

install(TARGETS ${koralle_LIB_NAME}
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

set(koralle_LIB_HDRS
    koralle_export.h
    fourcharcode.h
    riffstreamreader.h
)

install(FILES ${koralle_LIB_HDRS}
    DESTINATION include/${koralle_LIB_NAME}
)
