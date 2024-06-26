set(ORDERBOOK_SERVICE_FILES
    ${ROOT_DIR}/src/Orderbook/data_source.h
    ${ROOT_DIR}/src/Orderbook/itch.h
    ${ROOT_DIR}/src/Orderbook/msg_parser.h
    ${ROOT_DIR}/src/Orderbook/orderbook.cpp
    ${ROOT_DIR}/src/Orderbook/orderbook.h
    ${ROOT_DIR}/src/Orderbook/orderbook_reconstructor.h
    ${ROOT_DIR}/src/Orderbook/orders.h
    ${ROOT_DIR}/src/Orderbook/price_level.h
    ${ROOT_DIR}/src/Orderbook/side_book.h
    ${ROOT_DIR}/src/OrderbookService/orderbook_exe.cpp
)

add_executable(orderbook_exe
    ${ORDERBOOK_SERVICE_FILES}
)

target_include_directories(orderbook_exe PRIVATE
    ${ROOT_DIR}/src
)

set_target_properties(orderbook_exe PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

#add_dependencies(benchmark copy_data_files)
