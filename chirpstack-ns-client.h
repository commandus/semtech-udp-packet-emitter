#include <string>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "ns/ns.grpc.pb.h"
#include "as/external/api/device.grpc.pb.h"
#include "as/external/api/application.grpc.pb.h"

/**
 * @link https://www.chirpstack.io/application-server/api/
 * In order to use the gRPC API methods, you must provide per-RPC credentials.
 * The key for this metadata is authorization, the value Bearer <API TOKEN>
 * (replace <API TOKEN> with the API TOKEN obtained using the web-interface).
 * 
 **/
class ChirpstackNsClient {
	private:
		std::shared_ptr<grpc::ChannelCredentials> nsCredentials;
		// std::shared_ptr<grpc::ChannelCredentials> asCredentials;
		std::shared_ptr<grpc::ChannelCredentials> extCredentials;

		std::shared_ptr<grpc::Channel> channelNs;
		// std::shared_ptr<grpc::Channel> channelAs;
		std::shared_ptr<grpc::Channel> channelExt;
		
		std::unique_ptr<ns::NetworkServerService::Stub> nsClient;
		std::unique_ptr<api::DeviceService::Stub> devClient;
		std::unique_ptr<api::ApplicationService::Stub> appClient;
		bool stopRequest;
		std::string serviceNs;
		// std::string serviceAs;
		std::string serviceExt;
		std::string gatewayId;
		std::string deviceEUI;
		std::string token;

		std::function<void(
			ns::StreamFrameLogsForGatewayResponse &response
		)> onGateway;
		std::function<void(
			ns::StreamFrameLogsForDeviceResponse &response,
			const ns::Device &dev,
			const ns::DeviceActivation &devactivation
		)> onDevice;

		static void stdoutGatewayResponse(const ns::StreamFrameLogsForGatewayResponse &response);
		static void stdoutDeviceResponse(
			const ns::StreamFrameLogsForDeviceResponse &response,
			const ns::Device &dev,
			const ns::DeviceActivation &devactivation
		);

	public:
		bool daemonize;
		int verbosity;

		std::string logfilename;
	    std::ostream *logstream;

		ChirpstackNsClient();
		ChirpstackNsClient(std::function<void(const ns::StreamFrameLogsForGatewayResponse &response)> *onGateway);
		~ChirpstackNsClient();

		int connect();
		int disconnect();
		int run();

		bool setNsService(const std::string &value);
		bool setAsService(const std::string &value);
		bool setExtService(const std::string &value);
		bool setGatewayId(const std::string &value);
		bool setDeviceEUI(const std::string &value);
		bool setToken(const std::string &value);

		std::string version();
		bool listApplication(std::vector <api::ApplicationListItem> &retval, int64_t orgId);
		bool listDevices(std::vector <api::DeviceListItem> &retval, int64_t appId);
		bool getGateway(ns::Gateway &retval, const std::string &id);
		bool getDevice(ns::Device &retval, ns::DeviceActivation &retactivation, const std::string &eui);
		bool getDeviceActivation(ns::DeviceActivation &retval, const std::string &eui);
};
