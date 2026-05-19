add_executable(trading-system
    main.cpp
)

target_link_libraries(trading-system PRIVATE reference-data position_manager fix_session market_data)
