add_executable(trading-system
    main.cpp
)

target_link_libraries(trading-system PRIVATE reference-data order_book position_manager)
