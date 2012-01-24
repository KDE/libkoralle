set(koralle_LIB_CAMELCASE_HDRS
    FourCharCode
    RiffStreamReader
)

install(FILES ${koralle_LIB_CAMELCASE_HDRS}
    DESTINATION include/Koralle${koralle_LIB_ABI_VERSION}
)
