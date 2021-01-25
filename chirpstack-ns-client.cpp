#include <unistd.h>
#include <functional>

#include "chirpstack-ns-client.h"
#include "errlist.h"
#include "utilstring.h"
#include "utillora.h"

class ChirpstackAuthenticator : public grpc::MetadataCredentialsPlugin {
	private:
		grpc::string token;
	public:
  		ChirpstackAuthenticator(
			const grpc::string& apitoken)
		: token(apitoken) {

		}

  		grpc::Status GetMetadata(
    		grpc::string_ref service_url,
			grpc::string_ref method_name,
      		const grpc::AuthContext& channel_auth_context,
      		std::multimap<grpc::string, grpc::string>* metadata
		) override 
		{
			metadata->insert(std::make_pair("authorization", "Bearer " + token));
			return grpc::Status::OK;
		}
};

int ChirpstackNsClient::connect() {
	// Create a default SSL ChannelCredentials object
  	nsCredentials = grpc::InsecureChannelCredentials();
	/* 
	grpc::SslCredentialsOptions sslOpts;
	extCredentials = grpc::CompositeChannelCredentials(
		grpc::InsecureChannelCredentials(),
		// grpc::SslCredentials(sslOpts),
		grpc::MetadataCredentialsFromPlugin(
			std::unique_ptr<grpc::MetadataCredentialsPlugin>(new ChirpstackAuthenticator(token))
		)
	);
	*/
	extCredentials = grpc::InsecureChannelCredentials();
  	// Create a channel using the credentials created in the previous step.
  	channelNs = grpc::CreateChannel(serviceNs, nsCredentials);
	channelExt = grpc::CreateChannel(serviceExt, extCredentials);
  	// Create a stub on the channel.
  	nsClient = ns::NetworkServerService::NewStub(channelNs);
	devClient = api::DeviceService::NewStub(channelExt);
	appClient = api::ApplicationService::NewStub(channelExt);
}

int ChirpstackNsClient::disconnect() {
	stopRequest = true;
}

ChirpstackNsClient::ChirpstackNsClient() 
	: stopRequest(false), daemonize(false), verbosity(0),
	serviceNs(""), serviceExt(""), gatewayId(""), deviceEUI(""), token(""),
	logfilename(""), logstream(&std::cerr)
{
	onGateway = &ChirpstackNsClient::stdoutGatewayResponse;
	onDevice = &ChirpstackNsClient::stdoutDeviceResponse;
}

ChirpstackNsClient::ChirpstackNsClient(
	std::function<void(const ns::StreamFrameLogsForGatewayResponse &response)> *onGatewayCallback
)
	: stopRequest(false), daemonize(false), verbosity(0),
	serviceNs(""), serviceExt(""), gatewayId(""), deviceEUI(""), token(""),
	logfilename(""), logstream(&std::cerr)
{

}

ChirpstackNsClient::~ChirpstackNsClient() {
	disconnect();
}

bool ChirpstackNsClient::setNsService(
	const std::string &value
) {
	serviceNs = value;
	return true;
}

bool ChirpstackNsClient::setAsService(
	const std::string &value
) {
	// serviceAs = value;
	return true;
}

bool ChirpstackNsClient::setExtService(
	const std::string &value
) {
	serviceExt = value;
	return true;
}

bool ChirpstackNsClient::setGatewayId(
	const std::string &value
) {
	gatewayId = value;
	deviceEUI = "";
	return true;
}

bool ChirpstackNsClient::setDeviceEUI(
	const std::string &value
) {
	deviceEUI = value;
	gatewayId = "";
	return true;
}

bool ChirpstackNsClient::setToken(
	const std::string &value
) {
	token = value;
	return true;
}

std::string ChirpstackNsClient::version() {
	grpc::ClientContext context;
	google::protobuf::Empty request;
	ns::GetVersionResponse retval;
	grpc::Status r = nsClient->GetVersion(&context, request, &retval);
	if (r.ok()) {
		return retval.version();
	}
	return "";
}

bool ChirpstackNsClient::listApplication(
	std::vector <api::ApplicationListItem> &retval,
	int64_t orgId
) {
	grpc::ClientContext context;
	context.AddMetadata("authorization", "Bearer " + token);
	api::ListApplicationRequest request;
	request.set_organization_id(orgId);
	int64_t ofs = 0;
	int64_t limit = 1024;
	int64_t loaded = 0;
	while (true) {
		request.set_offset(ofs);
		request.set_limit(limit);
		api::ListApplicationResponse response;
		grpc::Status r = appClient->List(&context, request, &response);
		if (r.ok()) {
			int64_t count = response.total_count();
			int size = response.result_size();
			for (int i = 0; i < size; i++) {
				api::ApplicationListItem it = response.result(i);
				retval.push_back(it);
			}
			loaded += size;
			if (loaded >= count)
				break;
		} else {
			std::cerr << ERR_GRPC_NETWORK_SERVER_FAIL << r.error_code() << ": " << r.error_message() << std::endl;
			return false;
		}
	}
	return true;
}

bool ChirpstackNsClient::listDevices(
	std::vector <api::DeviceListItem> &retval,
	int64_t appId
) {
	grpc::ClientContext context;
	context.AddMetadata("authorization", "Bearer " + token);
	api::ListDeviceRequest request;
	request.set_application_id(appId);
	int64_t ofs = 0;
	int64_t limit = 1024;
	int64_t loaded = 0;
	while (true) {
		request.set_offset(ofs);
		request.set_limit(limit);
		api::ListDeviceResponse response;
		grpc::Status r = devClient->List(&context, request, &response);
		if (r.ok()) {
			int64_t count = response.total_count();
			int size = response.result_size();
			for (int i = 0; i < size; i++) {
				api::DeviceListItem it = response.result(i);
				retval.push_back(it);
			}
			loaded += size;
			if (loaded >= count)
				break;
		} else {
			std::cerr << ERR_GRPC_NETWORK_SERVER_FAIL << r.error_code() << ": " << r.error_message() << std::endl;
			return false;
		}
	}
	return true;
}

bool ChirpstackNsClient::getGateway(
	ns::Gateway &retval,
	const std::string &id
) {
	grpc::ClientContext context;
	ns::GetGatewayRequest request;
	request.set_id(hex2string(id));
	ns::GetGatewayResponse response;
	grpc::Status r = nsClient->GetGateway(&context, request, &response);
	if (r.ok()) {
		retval = response.gateway();
		return true;
	} else {
		std::cerr << ERR_GRPC_NETWORK_SERVER_FAIL << r.error_code() << ": " << r.error_message() << std::endl;
	}
	return false;
}

/**
 *  rpc GetDeviceActivation(GetDeviceActivationRequest) returns (GetDeviceActivationResponse)
 */
bool ChirpstackNsClient::getDeviceActivation(
	ns::DeviceActivation &retval,
	const std::string &eui
) {
	grpc::ClientContext context;
	ns::GetDeviceActivationRequest request;
	request.set_dev_eui(hex2string(eui));
	ns::GetDeviceActivationResponse response;
	grpc::Status r = nsClient->GetDeviceActivation(&context, request, &response);
	if (r.ok()) {
		retval = response.device_activation();
		return true;
	} else {
		std::cerr << ERR_GRPC_NETWORK_SERVER_FAIL << r.error_code() << ": " << r.error_message() << std::endl;
	}
	return false;
}

bool ChirpstackNsClient::getDevice(
	ns::Device &retval,
	ns::DeviceActivation &retactivation,
	const std::string &eui
) {
	grpc::ClientContext context;
	ns::GetDeviceRequest request;
	request.set_dev_eui(hex2string(eui));
	ns::GetDeviceResponse response;
	grpc::Status r = nsClient->GetDevice(&context, request, &response);
	if (r.ok()) {
		retval = response.device();
		return getDeviceActivation(retactivation, eui);
	} else {
		std::cerr << ERR_GRPC_NETWORK_SERVER_FAIL << r.error_code() << ": " << r.error_message() << std::endl;
	}
	return false;
}

/**			}

 * Default gateway printer
 */
void ChirpstackNsClient::stdoutGatewayResponse(
	const ns::StreamFrameLogsForGatewayResponse &response
) {
	std::string s;
	google::protobuf::util::MessageToJsonString(response, &s);
	std::cout << s << std::endl;
}

/**
 * Default device printer
 */
void ChirpstackNsClient::stdoutDeviceResponse(
	const ns::StreamFrameLogsForDeviceResponse &response,
	const ns::Device &dev,
	const ns::DeviceActivation &devactivation
) {
	std::string s;
	google::protobuf::util::MessageToJsonString(response, &s);
	std::cout << s << std::endl;
	if (response.has_uplink_frame_set()) {
		s = response.uplink_frame_set().phy_payload();
		std::cout << hexString(s) << std::endl;
		// semtechUDPPacket packet(s, "11111111", "11111111111111111111111111111111", 0);
		semtechUDPPacket packet(s, devactivation.dev_addr(), devactivation.s_nwk_s_int_key());
		std::cout << packet.toString() << std::endl;
		std::string s(packet.getPayload());
		std::cout 
			<< packet.getDeviceAddr()
			<< ": "
//			<< hexString(s)
			<< s
			<< std::endl;
	}
}

int ChirpstackNsClient::run() {
	int r = 0;
	grpc::ClientContext context;
	std::string ver = version();
	std::cerr << MSG_CHIRPSTACK_SERVER_VERSION << ver << std::endl;
	
	std::vector <api::ApplicationListItem> applications;
	if (!listApplication(applications, 0)) {
		return ERR_CODE_BAD_STATUS;
	}
	for (std::vector<api::ApplicationListItem>::iterator it(applications.begin()); it != applications.end(); it++) {
		std::cout << it->id() << ": " << it->name() << std::endl;
	}

	std::vector <api::DeviceListItem> devices;
	int64_t appId = 2;
	if (!listDevices(devices, appId)) {
		return ERR_CODE_BAD_STATUS;
	}
	for (std::vector<api::DeviceListItem>::iterator it(devices.begin()); it != devices.end(); it++) {
		std::cout << it->dev_eui() << ": " << it->name() << std::endl;
	}


	if (gatewayId.empty()) {
		// check does device exists
		ns::Device dev;
		ns::DeviceActivation devactivation;
		if (!getDevice(dev, devactivation, deviceEUI)) {
			return ERR_CODE_INVALID_DEVICE_EUI;
		} else {
			if (verbosity > 2) {
				std::string sdev;
				google::protobuf::util::MessageToJsonString(dev, &sdev);
				std::string sactivation;
				google::protobuf::util::MessageToJsonString(devactivation, &sactivation);
				std::cerr << MSG_DEVICE_EUI << ": " << deviceEUI << std::endl;
				std::cerr << MSG_DEVICE << ": " << sdev << std::endl;
				std::cerr << MSG_DEVICE_ACTIVATION << ": " << sactivation << std::endl;
			}
		}

		// read messages
		ns::StreamFrameLogsForDeviceRequest request;
		ns::StreamFrameLogsForDeviceResponse response;
		request.set_dev_eui(hex2string(deviceEUI));
		std::unique_ptr<grpc::ClientReader<ns::StreamFrameLogsForDeviceResponse>> reader(nsClient->StreamFrameLogsForDevice(&context, request));
		while (!stopRequest && reader->Read(&response)) {
			onDevice(response, dev, devactivation);
		}

		// finish
  		grpc::Status status = reader->Finish();
  		if (!status.ok())
    		return (int) status.error_code();
	} else {
		// check does gateway exists
		ns::Gateway gw;
		if (!getGateway(gw, gatewayId)) {
			return ERR_CODE_INVALID_GATEWAY_ID;
		} else {
			if (verbosity > 2) {
				std::string s;
				google::protobuf::util::MessageToJsonString(gw, &s);
				std::cerr << MSG_GATEWAY << gatewayId << ": " <<  s << std::endl;
			}
		}

		// read messages
		ns::StreamFrameLogsForGatewayRequest request;
		ns::StreamFrameLogsForGatewayResponse response;
		request.set_gateway_id(hex2string(gatewayId));
		std::unique_ptr<grpc::ClientReader<ns::StreamFrameLogsForGatewayResponse>> reader(nsClient->StreamFrameLogsForGateway(&context, request));
		while (!stopRequest && reader->Read(&response)) {
			onGateway(response);
		}

		// finish
  		grpc::Status status = reader->Finish();
  		if (!status.ok())
    		return (int) status.error_code();
  	}
	return LORA_OK;
}
