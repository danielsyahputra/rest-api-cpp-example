#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "SwaggerComponent.hpp"
#include "DatabaseComponent.hpp"
#include "ErrorHandler.hpp"
#include "ConfigurationComponent.hpp"

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/web/mime/ContentMappers.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/json/ObjectMapper.hpp"
#include "oatpp/macro/component.hpp"

class AppComponent {
    public:
        ConfigurationComponent configurationComponent;
        SwaggerComponent swaggerComponent;
        DatabaseComponent databaseComponent;

        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers)([] {

            auto json = std::make_shared<oatpp::json::ObjectMapper>();
            json->serializerConfig().json.useBeautifier = true;

            auto mappers = std::make_shared<oatpp::web::mime::ContentMappers>();
            mappers->putMapper(json);

            return mappers;

        }());

        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
            OATPP_COMPONENT(oatpp::Object<ConfigDto>, config);
            return oatpp::network::tcp::server::ConnectionProvider::createShared({config->host, config->port, oatpp::network::Address::IP_4});
        }());

        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
            return oatpp::web::server::HttpRouter::createShared();
        }());

        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {

            OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); 
            OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, contentMappers); 

            auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
            connectionHandler->setErrorHandler(std::make_shared<ErrorHandler>(contentMappers));
            return connectionHandler;

        }());
};

#endif