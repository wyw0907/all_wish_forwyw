#include "test.grpc.pb.h"
#include "health_check.grpc.pb.h"
#include <iostream>
#include <grpcpp/grpcpp.h>
#include <thread>
using test::Greeter;
using test::EchoRequest;
using test::EchoResponse;
using grpc::health::v1::HealthCheckRequest;
using grpc::health::v1::HealthCheckResponse;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;
class GreeterService : public Greeter::Service
{
public:
    Status SayHello(ServerContext *ctx, const EchoRequest * req, EchoResponse * rsp) override
    {
        std::cout << req->name() << std::endl;
        rsp->set_message(std::string("hellow ") + req->name());
        return Status();
    }
};

class HealthService : public grpc::health::v1::Health::Service
{
public:
    grpc::Status Check(grpc::ServerContext *ctx, const HealthCheckRequest *req, HealthCheckResponse *rsp) override
    {
        std::cout << req->service() << std::endl;
        rsp->set_status(HealthCheckResponse::SERVING);
        return grpc::Status();
    }

    grpc::Status Watch(grpc::ServerContext *ctx, const HealthCheckRequest *req, ServerWriter<HealthCheckResponse>* writer) override
    {
        std::cout << req->service() << std::endl;
        HealthCheckResponse rsp;
        rsp.set_status(HealthCheckResponse::SERVING);
        for (size_t i = 0; i < 10; i ++)
        {
            writer->Write(rsp);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        return grpc::Status();
    }
};

int main()
{
    std::string server_addr("0.0.0.0:5051");
    GreeterService service;
    HealthService hservice;
    grpc::EnableDefaultHealthCheckService(true);
    //grpc::reflection::
    ServerBuilder builder;
    builder.AddListeningPort(server_addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    //builder.RegisterService(&hservice);
    auto _server = builder.BuildAndStart();
    _server->Wait();
    return 0;
}
