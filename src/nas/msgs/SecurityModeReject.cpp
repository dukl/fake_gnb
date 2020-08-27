#include "SecurityModeReject.hpp"
#include "3gpp_ts24501.hpp"
#include "logger.hpp"

using namespace nas;

SecurityModeReject::SecurityModeReject() {
	Logger::nas_mm().debug("initiating class SecurityModeReject");
	plain_header = NULL;
	ie_5gmm_cause = NULL;
}

SecurityModeReject::~SecurityModeReject() {}

void SecurityModeReject::setHeader(uint8_t security_header_type) {
	plain_header = new NasMmPlainHeader();
	plain_header->setHeader(EPD_5GS_MM_MSG, security_header_type, SECURITY_MODE_REJECT);
}
void SecurityModeReject::set_5GMM_Cause(uint8_t value) {
	ie_5gmm_cause = new _5GMM_Cause(0x00,value);
}
int SecurityModeReject::encode2buffer(uint8_t *buf, int len) {
	Logger::nas_mm().debug("encoding SecurityModeReject message");
	int encoded_size = 0;
	if (!plain_header) {
		Logger::nas_mm().error("Mandontary IE missing Header");
		return 0;
	}
	if (!(plain_header->encode2buffer(buf, len))) return 0;
	encoded_size += 3;
	if (!ie_5gmm_cause) {
		Logger::nas_mm().warn("IE ie_5gmm_cause is not avaliable");
	}
	else {
		if (int size = ie_5gmm_cause->encode2buffer(buf + encoded_size, len - encoded_size)) {
			encoded_size += size;
		}
		else {
			Logger::nas_mm().error("encoding ie_5gmm_cause  error");
		}
	}
	Logger::nas_mm().debug("encoded SecurityModeReject message len(%d)", encoded_size);
	return 1;
}

int SecurityModeReject::decodefrombuffer(NasMmPlainHeader * header, uint8_t *buf, int len) {
	Logger::nas_mm().debug("decoding SecurityModeReject message");
	int decoded_size = 3;
	plain_header = header;
	ie_5gmm_cause = new _5GMM_Cause();
	decoded_size += ie_5gmm_cause->decodefrombuffer(buf + decoded_size, len - decoded_size, false);
	Logger::nas_mm().debug("decoded_size(%d)", decoded_size);
	uint8_t octet = *(buf + decoded_size);
	Logger::nas_mm().debug("first option iei(0x%x)", octet);
	Logger::nas_mm().debug("decoded SecurityModeReject message len(%d)", decoded_size);


}





