#include "health_check.grpc.pb.h"
#include <grpcpp/grpcpp.h>

class HealthService : public grpc::health::v1::Health::Service
{
public:
    grpc::Status Check(grpc::ServerContext *ctx, const grpc::health::v1::HealthCheckRequest *req, grpc::health::v1::HealthCheckResponse *rsp) override
    {
        std::cout << req->service() << std::endl;
        rsp->set_status(grpc::health::v1::HealthCheckResponse::SERVING);
        return grpc::Status();
    }
};

int main()
{
    std::string server_addr("0.0.0.0:9999");
    HealthService service;
    //grpc::EnableDefaultHealthCheckService(true);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    auto _server = builder.BuildAndStart();
    _server->Wait();
    return 0;
}
