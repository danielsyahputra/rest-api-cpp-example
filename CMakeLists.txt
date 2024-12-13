cmake_minimum_required(VERSION 3.20)
project(book_management_crud)

set(CMAKE_CXX_STANDARD 17)

add_library(crud-lib
        src/controller/StaticController.hpp
        src/controller/UserController.hpp
        src/controller/BookController.hpp
        src/controller/BorrowController.hpp
        src/db/UserDb.hpp
        src/db/BookDb.hpp
        src/db/BorrowDb.hpp
        src/dto/PageDto.hpp
        src/dto/StatusDto.hpp
        src/dto/UserDto.hpp
        src/dto/BookDto.hpp
        src/dto/BorrowDto.hpp
        src/dto/ConfigDto.hpp
        src/service/UserService.cpp
        src/service/UserService.hpp
        src/service/BookService.hpp
        src/service/BorrowService.hpp
        src/service/BookService.cpp
        src/service/BorrowService.cpp
        src/AppComponent.hpp
        src/DatabaseComponent.hpp
        src/SwaggerComponent.hpp
        src/ErrorHandler.cpp
        src/ErrorHandler.hpp
        src/ConfigurationComponent.hpp
)


target_include_directories(crud-lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")


find_package(oatpp              1.4.0 REQUIRED)
find_package(oatpp-swagger      1.4.0 REQUIRED)
find_package(oatpp-sqlite       1.4.0 REQUIRED)
find_package(oatpp-postgresql   1.4.0 REQUIRED)

target_link_libraries(crud-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
        PUBLIC oatpp::oatpp-sqlite
        PUBLIC oatpp::oatpp-postgresql
)


add_definitions(
        -DOATPP_SWAGGER_RES_PATH="/usr/local/include/oatpp-1.4.0/bin/oatpp-swagger/res/"

        ## SQLite database file
        -DDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/db.sqlite"

        -DTESTDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/test-db.sqlite"

        -DDATABASE_MIGRATIONS="${CMAKE_CURRENT_SOURCE_DIR}/sql"
)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    find_package(Threads REQUIRED)
    target_link_libraries(crud-lib INTERFACE Threads::Threads ${CMAKE_DL_LIBS})
endif()

add_executable(crud-exe src/App.cpp)
target_link_libraries(crud-exe crud-lib)

add_executable(crud-test
        tests/tests.cpp
        tests/app/TestClient.hpp
        tests/app/TestDatabaseComponent.hpp
        tests/app/TestComponent.hpp
        tests/UserControllerTest.hpp
        tests/UserControllerTest.cpp
        tests/BookControllerTest.hpp
        tests/BookControllerTest.cpp
        tests/BorrowControllerTest.hpp
        tests/BorrowControllerTest.cpp
)
target_link_libraries(crud-test crud-lib)

enable_testing()
add_test(project-tests crud-test)