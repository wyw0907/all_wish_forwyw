#include "test.grpc.pb.h"
#include "health_check.grpc.pb.h"

#include <iostream>
#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::health::v1::HealthCheckRequest;
using grpc::health::v1::HealthCheckResponse;
using test::EchoRequest;
using test::EchoResponse;
using test::Greeter;
int main()
{
    auto conn = grpc::CreateChannel("127.0.0.1:5051", grpc::InsecureChannelCredentials());
    auto stub = Greeter::NewStub(conn);
    EchoRequest req;
    req.set_name("wyw");

    EchoResponse rsp;
    ClientContext ctx;
    auto status = stub->SayHello(&ctx, req, &rsp);
    if (status.ok())
    {
        std::cout << "SayHello: " << rsp.message() << std::endl;
    }
    else
    {
        std::cout << "exception: " << status.error_message() << std::endl;
    }

    {
        auto hstub = grpc::health::v1::Health::NewStub(conn);
        HealthCheckRequest hreq;
        hreq.set_service("test");

        ClientContext ctx1;
        HealthCheckResponse hrsp;
        auto status1 = hstub->Check(&ctx1, hreq, &hrsp);
        if (status1.ok())
        {
            std::cout << "Check: " << hrsp.status() << std::endl;
        }
        else
        {
            std::cout << "exception: " << status.error_message() << std::endl;
        }

        ClientContext ctx2;
        auto reader = hstub->Watch(&ctx2, hreq);
        while(reader->Read(&hrsp))
        {
            std::cout << "Watch: " << hrsp.status() << std::endl;
        }
        auto status2 = reader->Finish();
        if (status2.ok())
        {
            std::cout << "Watch: " << hrsp.status() << std::endl;
        }
        else
        {
            std::cout << "exception: " << status.error_message() << std::endl;
        }
    }
}