
set(ORDERBOOK_SERVICE_FILES
    ${ROOT_DIR}/src/OrderbookService/orderbook_exe.cpp
)

add_executable(orderbook_exe
    ${ORDERBOOK_SERVICE_FILES}
)

set_target_properties(orderbook_exe PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

#add_dependencies(benchmark copy_data_files)
