#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#include <errno.h>
#include <cstdlib>

#include "chirpstack-ns-client.h"

#include "argtable3/argtable3.h"
#include "platform.h"
#include "utilstring.h"
#include "daemonize.h"

#include "errlist.h"
#include "utillora.h"
#include "utilstring.h"

const std::string progname = "chirpstack-ns-log";
#define DEF_CONFIG_FILE_NAME ".chirpstack-ns-log"
#define DEF_TIME_FORMAT "%FT%T"
#define DEF_NS_SERVICE	"localhost:8000"
#define DEF_AS_SERVICE	"localhost:8001"
#define DEF_EXT_SERVICE	"localhost:8080"

static ChirpstackNsClient *client = NULL;

static void done()
{
	// destroy and free all
	if (client) {
		if (client->verbosity > 1)
			std::cerr << MSG_STOPPED << std::endl;
		delete client;
	}
	client = NULL;
	exit(0);
}

static void stop()
{
	if (client)
		client->disconnect();
}

void signalHandler(int signal)
{
	switch (signal)
	{
	case SIGINT:
		std::cerr << MSG_INTERRUPTED << std::endl;
		stop();
		done();
		break;
	default:
		break;
	}
}

#ifdef _MSC_VER
// TODO
void setSignalHandler()
{
}
#else
void setSignalHandler()
{
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = &signalHandler;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGHUP, &action, NULL);
}
#endif

/**
 * Parse command line
 * Return 0- success
 *        1- show help and exit, or command syntax error
 *        2- output file does not exists or can not open to write
 **/
int parseCmd(
	ChirpstackNsClient *client,
	int argc,
	char *argv[])
{
	// service 
  	struct arg_str *a_ns_service = arg_str0("n", "ns", "host:port", "Default " DEF_NS_SERVICE);
	struct arg_str *a_as_service = arg_str0("a", "as", "host:port", "Default " DEF_AS_SERVICE);
	struct arg_str *a_ex_service = arg_str0("e", "ext", "host:port", "Default " DEF_EXT_SERVICE);
	struct arg_str *a_gateway_id = arg_str0("g", "gateway", "id", "e.g. 00006cc3743eed46");
	struct arg_str *a_device_eui = arg_str0("d", "device", "eui", "e.g. 1122334455667788");
	struct arg_str *a_apikey = arg_str0("k", "key", "key", "API key string e.g. eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.e...");
	struct arg_str *a_apikey_file = arg_str0("K", "keyfile", "file", "API key file name e,g, api.key");

	struct arg_str *a_logfilename = arg_str0("l", "logfile", "<file>", "log file");
	struct arg_lit *a_daemonize = arg_lit0("d", "daemonize", "run daemon");
	struct arg_lit *a_verbosity = arg_litn("v", "verbose", 0, 3, "Set verbosity level");
	struct arg_lit *a_help = arg_lit0("?", "help", "Show this help");
	struct arg_end *a_end = arg_end(20);

	void *argtable[] = {
		a_ns_service, a_as_service, a_ex_service, a_gateway_id, a_device_eui, a_apikey, a_apikey_file,
		a_logfilename, a_daemonize, a_verbosity, a_help, a_end };

	int nerrors;

	// verify the argtable[] entries were allocated successfully
	if (arg_nullcheck(argtable) != 0)
	{
		arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
		return 1;
	}
	// Parse the command line as defined by argtable[]
	nerrors = arg_parse(argc, argv, argtable);

	client->daemonize = a_daemonize->count > 0;
	client->verbosity = a_verbosity->count;
	if (a_ns_service->count)
	{
		std::string s(*a_ns_service->sval);
		if (s.length() == 0)
		{
			std::cerr << ERR_INVALID_SERVICE << std::endl;
			nerrors++;
		}
		else
		{
			client->setNsService(s);
		}
	} else {
		client->setNsService(DEF_NS_SERVICE);
	}

	if (a_as_service->count)
	{
		std::string s(*a_as_service->sval);
		if (s.length() == 0)
		{
			std::cerr << ERR_INVALID_SERVICE << std::endl;
			nerrors++;
		}
		else
		{
			client->setAsService(s);
		}
	} else {
		client->setAsService(DEF_AS_SERVICE);
	}

	if (a_ex_service->count)
	{
		std::string s(*a_ex_service->sval);
		if (s.length() == 0)
		{
			std::cerr << ERR_INVALID_SERVICE << std::endl;
			nerrors++;
		}
		else
		{
			client->setExtService(s);
		}
	} else {
		client->setExtService(DEF_EXT_SERVICE);
	}

	if (a_apikey_file->count) {
		std::string s = file2string(*a_apikey_file->sval);
		client->setToken(trim(s));
	} else {
		if (a_apikey->count) {
			client->setToken(*a_apikey->sval);
		}
	}

	if (a_gateway_id->count)
	{
		std::string s(*a_gateway_id->sval);
		if (s.length() == 0)
		{
			std::cerr << ERR_INVALID_GATEWAY_ID << std::endl;
			nerrors++;
		}
		else
		{
			client->setGatewayId(s);
		}
	}

	if (a_device_eui->count)
	{
		std::string s(*a_device_eui->sval);
		if (s.length() == 0)
		{
			std::cerr << ERR_INVALID_DEVICE_EUI << std::endl;
			nerrors++;
		}
		else
		{
			client->setDeviceEUI(s);
		}
	}

	if (a_logfilename->count)
	{
		client->logfilename = *a_logfilename->sval;
		client->logstream = new std::fstream(*a_logfilename->sval, std::ostream::out);
		if (!client->logstream || client->logstream->bad())
		{
			std::cerr << ERR_INVALID_PAR_LOG_FILE << std::endl;
			nerrors++;
			if (client->logstream)
			{
				delete client->logstream;
				client->logstream = NULL;
			}
		}
	}
	else
	{
		client->logstream = &std::cerr;
		client->logfilename = "";
	}

	// special case: '--help' takes precedence over error reporting
	if ((a_help->count) || nerrors)
	{
		if (nerrors)
			arg_print_errors(stderr, a_end, progname.c_str());
		std::cerr << "Usage: " << progname << std::endl;
		arg_print_syntax(stderr, argtable, "\n");
		std::cerr << "UDP client" << std::endl;
		arg_print_glossary(stderr, argtable, "  %-25s %s\n");
		arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
		return 1;
	}

	arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
	return 0;
}

static void run()
{
	if (client) {
		client->connect();
		int r = client->run();
		if (r) {
			std::cerr << "Error " << r << ": " << strerror_client(r) << std::endl;
		}
	}
}

int main(
	int argc,
	char *argv[]
)
{
	client = new ChirpstackNsClient();

	if (parseCmd(client, argc, argv) != 0)
	{
		exit(ERR_CODE_COMMAND_LINE);
	}
#ifdef _MSC_VER
#else
	setSignalHandler();
#endif

	if (client->daemonize)
	{
		char wd[PATH_MAX];
		std::string progpath = getcwd(wd, PATH_MAX);
		if (client->verbosity > 1)
			std::cerr << MSG_DAEMON_STARTED << progpath << "/" << progname << MSG_DAEMON_STARTED_1 << std::endl;
		OPENSYSLOG()
		Daemonize daemonize(progname, progpath, run, stop, done);
		// CLOSESYSLOG()
	}
	else
	{
		setSignalHandler();
		run();
		done();
	}
	return 0;
}
