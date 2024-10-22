#ifndef UserController_hpp
#define UserController_hpp

#include "service/UserService.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/web/mime/ContentMappers.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

class UserController: public oatpp::web::server::api::ApiController {
    public:
        UserController(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers))
        : oatpp::web::server::api::ApiController(apiContentMappers)
        {}
    private:
        UserService userService;
    public:
        static std::shared_ptr<UserController> createShared(
            OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers)
        ){
            return std::make_shared<UserController>(apiContentMappers);
        }

        ENDPOINT_INFO(createUser) {
            info->summary = "Create new User";

            info->addConsumes<Object<UserDto>>("application/json");

            info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
        }
        ENDPOINT("POST", "users", createUser, BODY_DTO(oatpp::Object<UserDto>, userDto)) {
            return createDtoResponse(Status::CODE_200, userService.createUser(userDto));
        }

        ENDPOINT_INFO(putUser) {
            info->summary = "Update User by userId";

            info->addConsumes<Object<UserDto>>("application/json");

            info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

            info->pathParams["userId"].description = "User Identifier";
        }
        ENDPOINT("PUT", "users/{userId}", putUser,
                PATH(Int32, userId),
                BODY_DTO(Object<UserDto>, userDto))
        {
            userDto->id = userId;
            return createDtoResponse(Status::CODE_200, userService.updateUser(userDto));
        }


        ENDPOINT_INFO(getUserById) {
            info->summary = "Get one User by userId";

            info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

            info->pathParams["userId"].description = "User Identifier";
        }
        ENDPOINT("GET", "users/{userId}", getUserById,
            PATH(Int32, userId))
        {
            return createDtoResponse(Status::CODE_200, userService.getUserById(userId));
        }


        ENDPOINT_INFO(getUsers) {
            info->summary = "get all stored users";

            info->addResponse<oatpp::Object<UsersPageDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
        }
        ENDPOINT("GET", "users/offset/{offset}/limit/{limit}", getUsers,
                PATH(UInt32, offset),
                PATH(UInt32, limit))
        {
            return createDtoResponse(Status::CODE_200, userService.getAllUsers(offset, limit));
        }


        ENDPOINT_INFO(deleteUser) {
            info->summary = "Delete User by userId";

            info->addResponse<Object<StatusDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

            info->pathParams["userId"].description = "User Identifier";
        }
        ENDPOINT("DELETE", "users/{userId}", deleteUser,
            PATH(Int32, userId))
        {
            return createDtoResponse(Status::CODE_200, userService.deleteUserById(userId));
        }

};

#include OATPP_CODEGEN_END(ApiController)

#endif