#include "AppComponent.hpp"
#include "controller/UserController.hpp"
#include "controller/StaticController.hpp"
#include "controller/BookController.hpp"
#include "controller/BorrowController.hpp"

#include "oatpp-swagger/Controller.hpp"
#include "oatpp/network/Server.hpp"

#include <iostream>

void run() {
    AppComponent components;

    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    oatpp::web::server::api::Endpoints docEndpoints;

    docEndpoints.append(router->addController(UserController::createShared())->getEndpoints());
    docEndpoints.append(router->addController(BookController::createShared())->getEndpoints());
    docEndpoints.append(router->addController(BorrowController::createShared())->getEndpoints());

    router->addController(oatpp::swagger::Controller::createShared(docEndpoints));
    router->addController(StaticController::createShared());

    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

    oatpp::network::Server server(connectionProvider, connectionHandler);
  
    OATPP_LOGd("Server", "Running on port {}...", connectionProvider->getProperty("port").toString())
    
    server.run();

    OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);
    dbConnectionProvider->stop();
}

int main(int argc, const char * argv[]) {
    oatpp::Environment::init();
    
    run();

    std::cout << "\nEnvironment:\n";
    std::cout << "objectsCount = " << oatpp::Environment::getObjectsCount() << "\n";
    std::cout << "objectsCreated = " << oatpp::Environment::getObjectsCreated() << "\n\n";
    
    oatpp::Environment::destroy();
    
    return 0;
}