#include "nas_context.hpp"

nas_context::nas_context(){
  security_ctx = NULL;
  is_imsi_present = false;
  is_5g_guti_present = false;
  to_be_register_by_new_suci = false;
  is_stacs_available = false;
  is_auth_vectors_present = false;
  is_specific_procedure_for_registration_running = false;
  is_specific_procedure_for_deregistration_running = false;
  is_specific_procedure_for_eCell_inactivity_running = false;
  is_common_procedure_for_authentication_running = false;
  is_common_procedure_for_identification_running = false;
  is_common_procedure_for_security_mode_control_running = false;
  is_common_procedure_for_nas_transport_running = false;
  auts = NULL;
}

nas_context::~nas_context(){}
